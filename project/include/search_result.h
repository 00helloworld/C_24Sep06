#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include "dict.h" // 包含 Suburb 


typedef struct SearchResult {
    int node_accesses;     // 访问的链表节点数
    int bit_comparisons;   // 二进制比较次数
    int string_comparisons; // 字符串比较次数
    int found; // 是否找到匹配的suburb
    struct Suburb *closest_suburb; // 新增的 Suburb 指针，存储找到的最接近的 Suburb
} SearchResult;

#endif
