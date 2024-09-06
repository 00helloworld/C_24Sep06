#ifndef DICT_H
#define DICT_H

#include "search_result.h"


// suburb 的结构体
typedef struct Suburb {
    int comp20003_code;
    int official_code_suburb;
    char *official_name_suburb;
    int year;
    char *official_code_state;
    char *official_name_state;
    char *official_code_local_government_area;
    char *official_name_local_government_area;
    double latitude;
    double longitude;
    struct Suburb *next;  // 用于指向链表中的下一个 suburb
} Suburb;

// Patricia Tree 节点结构体
typedef struct PatriciaNode {
    char *key;                        // 节点的键
    struct PatriciaNode *left;        // 左子节点
    struct PatriciaNode *right;       // 右子节点
    Suburb *suburb_data;              // 指向与该键相关联的 suburb 数据
    int is_end;                       // 是否为最后一个字符
} PatriciaNode;

// 声明
Suburb *load_data(const char *filename);
void search_by_suburb_name(Suburb *head, const char *suburb_name, FILE *output_file);
void delete_by_suburb_name(Suburb **head, const char *suburb_name, FILE *output_file);
void free_data(Suburb *head);
PatriciaNode *insert_patricia(PatriciaNode *root, const char *key, Suburb *suburb_data);
SearchResult search_patricia(PatriciaNode *root, const char *key);
void free_patricia(PatriciaNode *root);
SearchResult fuzzy_search_patricia(PatriciaNode *root, const char *key);  // 新增的模糊搜索声明

#endif
