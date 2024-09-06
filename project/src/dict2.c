#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s 2 <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    // 读取数据
    Suburb *head = load_data(argv[2]);

    FILE *output_file = fopen(argv[3], "w");

    // 读取查询
    char suburb_name[128];
    while (fgets(suburb_name, sizeof(suburb_name), stdin)) {
        suburb_name[strcspn(suburb_name, "\n")] = 0;
        delete_by_suburb_name(&head, suburb_name, output_file);
    }

    // 所有删除操作完成后，将剩余的 suburb 数据写入 output.txt
    Suburb *current = head;
    fprintf(output_file, "COMP20003 Code,Official Code Suburb,Official Name Suburb,Year,Official Code State,Official Name State,Official Code Local Government Area,Official Name Local Government Area,Latitude,Longitude\n");
    while (current != NULL) {
        fprintf(output_file, "%d,%d,%s,%d", 
            current->comp20003_code,
            current->official_code_suburb,
            current->official_name_suburb,
            current->year);
        if (strchr(current->official_code_state, ',') != NULL) {
            
            fprintf(output_file, ",\"%s\"", current->official_code_state);
        } else {
            fprintf(output_file, ",%s", current->official_code_state);
        }
        if (strchr(current->official_name_state, ',') != NULL) {
            
            fprintf(output_file, ",\"%s\"", current->official_name_state);
        } else {
            fprintf(output_file, ",%s", current->official_name_state);
        }
        if (strchr(current->official_code_local_government_area, ',') != NULL) {
            fprintf(output_file, ",\"%s\"", current->official_code_local_government_area);
        } else {
            fprintf(output_file, ",%s", current->official_code_local_government_area);
        }
        if (strchr(current->official_name_local_government_area, ',') != NULL) {
            fprintf(output_file, ",\"%s\"", current->official_name_local_government_area);
        } else {
            fprintf(output_file, ",%s", current->official_name_local_government_area);
        }
        fprintf(output_file, ",%lf,%lf\n", 
            current->latitude,
            current->longitude);
        
        current = current->next;
    }

    fclose(output_file);
    return 0;
}

