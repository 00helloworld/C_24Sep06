#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"
#include "search_result.h"

// 获取某个字符的特定位
int getBit(const char *key, unsigned int bitIndex) {
    unsigned int byteIndex = bitIndex / 8;
    unsigned int bitOffset = 7 - (bitIndex % 8);
    return (key[byteIndex] >> bitOffset) & 1;
}

// 创建一个 Patricia Tree 节点
PatriciaNode *create_patricia_node(const char *key, int is_end) {
    PatriciaNode *node = (PatriciaNode *)malloc(sizeof(PatriciaNode));
    node->key = strdup(key);
    node->left = node->right = NULL;
    node->is_end = is_end;
    node->suburb_data = NULL; // 初始化为 NULL
    return node;
}

// 插入一个键到 Patricia Tree 中
PatriciaNode *insert_patricia(PatriciaNode *root, const char *key, Suburb *suburb_data) {
    if (root == NULL) {
        PatriciaNode *node = create_patricia_node(key, 1);
        node->suburb_data = suburb_data;  // 将 suburb 数据存入节点
        return node;
    }

    PatriciaNode *current = root;
    unsigned int bitIndex = 0;

    // 遍历树，直到找到插入位置
    while (current->left != NULL || current->right != NULL) {
        int bit = getBit(key, bitIndex);

        if (bit == 0) {
            if (current->left == NULL) break;
            current = current->left;
        } else {
            if (current->right == NULL) break;
            current = current->right;
        }
        bitIndex++;
    }

    // 查找位置后插入新节点
    if (strcmp(current->key, key) == 0) {
        current->is_end = 1;  // 已存在该键，标记为终止节点
        current->suburb_data = suburb_data;  // 更新节点的 suburb 数据
    } else {
        PatriciaNode *newNode = create_patricia_node(key, 1);
        newNode->suburb_data = suburb_data;  // 设置新节点的 suburb 数据
        int bit = getBit(key, bitIndex);
        if (bit == 0) {
            current->left = newNode;
        } else {
            current->right = newNode;
        }
    }
    
    return root;
}

// 查找 Patricia 键
SearchResult search_patricia(PatriciaNode *root, const char *key) {
    SearchResult result = {0, 0, 0, 0, NULL};  // 初始化搜索结果
    PatriciaNode *current = root;
    unsigned int bitIndex = 0;

    while (current != NULL) {
        result.node_accesses++;
        result.string_comparisons++;

        if (strcmp(current->key, key) == 0 && current->is_end) {
            result.found = 1;
            result.closest_suburb = current->suburb_data; // 如果找到匹配项，设置 suburb 数据
            return result;
        }

        int bit = getBit(key, bitIndex);
        result.bit_comparisons++;

        if (bit == 0) {
            current = current->left;
        } else {
            current = current->right;
        }
        bitIndex++;
    }

    result.found = 0;  // 没有找到匹配项
    return result;
}

// Edit Distance 计算函数（基于 Levenshtein Distance 算法）
int editDistance(const char *str1, const char *str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int dp[len1 + 1][len2 + 1];

    // 初始化dp数组
    for (int i = 0; i <= len1; i++) {
        for (int j = 0; j <= len2; j++) {
            if (i == 0) {
                dp[i][j] = j;  // 第一个字符串为空时的编辑距离
            } else if (j == 0) {
                dp[i][j] = i;  // 第二个字符串为空时的编辑距离
            } else if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];  // 字符相等，不做任何操作
            } else {
                dp[i][j] = 1 + (dp[i - 1][j - 1] < dp[i - 1][j] ? 
                                (dp[i - 1][j - 1] < dp[i][j - 1] ? dp[i - 1][j - 1] : dp[i][j - 1])
                                : (dp[i - 1][j] < dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1]));
                // 替换、删除或插入操作
            }
        }
    }
    return dp[len1][len2];  // 返回编辑距离
}

// 模糊搜索 Patricia 树的辅助递归函数
void searchClosestMatch(PatriciaNode *current, const char *key, SearchResult *result, int *min_distance, const char **best_match) {
    if (current == NULL) {
        return;
    }

    // 计算当前节点的 edit distance
    int distance = editDistance(current->key, key);
    if (distance < *min_distance) {
        *min_distance = distance;
        *best_match = current->key;
        result->closest_suburb = current->suburb_data; // 设置最接近的 suburb
        result->found = 1;
    }

    result->node_accesses++;
    result->string_comparisons++;

    // 递归查找左、右子树
    searchClosestMatch(current->left, key, result, min_distance, best_match);
    searchClosestMatch(current->right, key, result, min_distance, best_match);
}

// 模糊搜索 Patricia 树
SearchResult fuzzy_search_patricia(PatriciaNode *root, const char *key) {
    SearchResult result = search_patricia(root, key);
    
    // 如果没有找到完全匹配的项，进行模糊搜索
    if (!result.found) {
        int min_distance = 1000;  // 初始化为一个足够大的值
        const char *best_match = NULL;
        
        // 调用递归函数，查找最接近的匹配项
        searchClosestMatch(root, key, &result, &min_distance, &best_match);
        
        if (best_match != NULL) {
            printf("Closest match found: %s (edit distance: %d)\n", best_match, min_distance);
        }
    }
    
    return result;
}

// 递归释放 Patricia 
void free_patricia(PatriciaNode *root) {
    if (root == NULL) {
        return;
    }
    free_patricia(root->left);
    free_patricia(root->right);

    free(root->key);  // 释放 key 的内存
    if (root->suburb_data) {
        free(root->suburb_data->official_name_suburb);  // 释放动态分配的字符串
        free(root->suburb_data->official_code_state);
        free(root->suburb_data->official_name_state);
        free(root->suburb_data->official_code_local_government_area);
        free(root->suburb_data->official_name_local_government_area);
        free(root->suburb_data);  // 释放 Suburb 数据
    }

    free(root);  // 释放树节点
}
