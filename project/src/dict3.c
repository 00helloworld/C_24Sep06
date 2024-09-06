#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dict.h"
#include "search_result.h"

// 搜索函数，包含比较计数功能
SearchResult search_with_comparison(Suburb *head, const char *suburb_name) {
    Suburb *current = head;
    SearchResult result = {0, 0, 0};  // 初始化

    while (current != NULL) {
        result.node_accesses++;  // 访问链表节点次数

        for (int i = 0; suburb_name[i] != '\0' && current->official_name_suburb[i] != '\0'; i++) {
            result.bit_comparisons += 8;  // 每个字符8位比较
            result.string_comparisons++;  // 字符串比较计数
            if (suburb_name[i] != current->official_name_suburb[i]) {
                break;
            }
        }

        if (strcmp(suburb_name, current->official_name_suburb) == 0) {
            result.found = 1;  // 找到匹配项
            return result;
        }

        // 移动到下一个链表节点
        current = current->next;
    }

    result.found = 0;  // 未找到匹配项
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <stage> <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    // 文件
    Suburb *head = load_data(argv[2]);
    FILE *output_file = fopen(argv[3], "w");
    if (!output_file) {
        printf("Could not open output file %s\n", argv[3]);
        return 1;
    }

    char query[128];
    SearchResult result = {0, 0, 0};  // 初始化搜索结果

    // 读取查询
    while (fgets(query, sizeof(query), stdin)) {
        query[strcspn(query, "\n")] = 0;  // 去掉换行符

        result = search_with_comparison(head, query);

        // 输出suburb name
        fprintf(output_file, "%s -->\n", query);

        int record_count = 0;
        if (result.found > 0) {
            Suburb *current = head;
            while (current != NULL) {
                if (strcmp(current->official_name_suburb, query) == 0) {
                    fprintf(output_file, "COMP20003 Code: %d,Official Code Suburb: %d,Official Name Suburb: %s,Year: %d ", 
                        current->comp20003_code,
                        current->official_code_suburb,
                        current->official_name_suburb,
                        current->year);
                    if (strchr(current->official_code_state, ',') != NULL) {
                        fprintf(output_file, ",Official Code State: \"%s\"", current->official_code_state);
                    } else {
                        fprintf(output_file, ",Official Code State: %s", current->official_code_state);
                    }
                    if (strchr(current->official_name_state, ',') != NULL) {
                        fprintf(output_file, ",Official Name State: \"%s\"", current->official_name_state);
                    } else {
                        fprintf(output_file, ",Official Name State: %s", current->official_name_state);
                    }
                    if (strchr(current->official_code_local_government_area, ',') != NULL) {
                        fprintf(output_file, ",Official Code Local Government Area: \"%s\"", current->official_code_local_government_area);
                    } else {
                        fprintf(output_file, ",Official Code Local Government Area: %s", current->official_code_local_government_area);
                    }
                    if (strchr(current->official_name_local_government_area, ',') != NULL) {
                        fprintf(output_file, ",Official Name Local Government Area: \"%s\"", current->official_name_local_government_area);
                    } else {
                        fprintf(output_file, ",Official Name Local Government Area: %s", current->official_name_local_government_area);
                    }
                    fprintf(output_file, ",Latitude: %lf,Longitude: %lf\n", 
                        current->latitude,
                        current->longitude);
                    record_count++;
                }
                current = current->next;
            }
            printf("%s --> %d records - comparisons: b%d n%d s%d\n", 
                query, 
                record_count, 
                result.bit_comparisons, 
                result.node_accesses, 
                result.string_comparisons);
        } else {
            // fprintf(output_file, "%s --> NOTFOUND\n", query);
            printf("%s --> NOTFOUND\n", query);
        }

    }

    fclose(output_file);
    free_data(head);

    return 0;
}
