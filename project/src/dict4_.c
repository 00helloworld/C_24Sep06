#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"
#include "search_result.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <stage> <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    Suburb *head = load_data(argv[2]);
    FILE *output_file = fopen(argv[3], "w");
    if (!output_file) {
        printf("Could not open output file %s\n", argv[3]);
        return 1;
    }

    char query[128];
    SearchResult result = {0, 0, 0, 0};  // 初始化搜索结果
    PatriciaNode *patricia_root = NULL; // Patricia根节点

    // 插入Patricia
    Suburb *current = head;
    while (current != NULL) {
        patricia_root = insert_patricia(patricia_root, current->official_name_suburb);
        current = current->next;
    }

    // 读取查询
    while (fgets(query, sizeof(query), stdin)) {
        query[strcspn(query, "\n")] = 0;  // 去掉换行符

        result = search_patricia(patricia_root, query);

        // 输出
        fprintf(output_file, "%s --> \n", query);
        if (result.found) {
            Suburb *current = head;
            while (current != NULL) {
                if (strcmp(current->official_name_suburb, query) == 0) {
                    fprintf(output_file, "COMP20003 Code: %d,Official Code Suburb: %d,Official Name Suburb: %s,Year: %d,Official Code State: %s,Official Name State: %s,Official Code Local Government Area: %s,Official Name Local Government Area: %s,Latitude: %lf,Longitude: %lf\n", 
                        current->comp20003_code,
                        current->official_code_suburb,
                        current->official_name_suburb,
                        current->year,
                        current->official_code_state,  
                        current->official_name_state,
                        current->official_code_local_government_area,  
                        current->official_name_local_government_area,
                        current->latitude,
                        current->longitude);
                }
                current = current->next;
            }
            printf("%s --> %d records found - comparisons: b%d n%d s%d\n", query, result.found, result.bit_comparisons, result.node_accesses, result.string_comparisons);
        } else {
            printf("%s --> NOTFOUND\n", query);
        }
    }

    fclose(output_file);
    free_data(head);
    free_patricia(patricia_root);

    return 0;
}
