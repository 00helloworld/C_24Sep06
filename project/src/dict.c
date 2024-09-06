#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"


// 处理引号和逗号
char *parse_field(char **line) {
    char *start = *line;
    char *result;
    
    if (*start == '"') {
        // 处理引号包裹的内容
        start++;  
        char *end = strchr(start, '"');
        if (end) {
            *end = '\0';  
            *line = end + 2; 
        }
        result = strdup(start);
    } else {
        // 没有引号的字段
        char *end = strchr(start, ',');
        if (end) {
            *end = '\0';
            *line = end + 1;
        } else {
            *line = start + strlen(start);
        }
        result = strdup(start);
    }

    return result;
}

// 加载数据
Suburb *load_data(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file %s\n", filename);
        return NULL;
    }

    Suburb *head = NULL, *current = NULL;

    // 忽略表头
    char buffer[512];
    fgets(buffer, sizeof(buffer), file);

    // 读取每一行
    while (fgets(buffer, sizeof(buffer), file)) {
        Suburb *new_node = (Suburb *)malloc(sizeof(Suburb));

        char *line = buffer;

        new_node->comp20003_code = atoi(parse_field(&line));
        new_node->official_code_suburb = atoi(parse_field(&line));
        new_node->official_name_suburb = parse_field(&line);
        new_node->year = atoi(parse_field(&line));
        new_node->official_code_state = parse_field(&line);  // 按字符串处理
        new_node->official_name_state = parse_field(&line);
        new_node->official_code_local_government_area = parse_field(&line);  // 按字符串处理
        new_node->official_name_local_government_area = parse_field(&line);
        
        new_node->latitude = atof(parse_field(&line));
        new_node->longitude = atof(parse_field(&line));

        new_node->next = NULL;
        if (head == NULL) {
            head = new_node;
        } else {
            current->next = new_node;
        }
        current = new_node;
    }

    fclose(file);
    return head;
}



// 按 suburb name 查找
void search_by_suburb_name(Suburb *head, const char *suburb_name, FILE *output_file) {
    Suburb *current = head;
    int found = 0;
    int record_count = 0;

    fprintf(output_file, "%s -->\n", suburb_name);  

    while (current != NULL) {
        if (strcmp(current->official_name_suburb, suburb_name) == 0) {
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
            record_count++;
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        fprintf(output_file, "NOTFOUND\n"); 
    }

    // 输出记录数量
    if (found) {
        fprintf(stdout, "%s --> %d records found\n", suburb_name, record_count);
    } else {
        fprintf(stdout, "%s --> NOTFOUND\n", suburb_name);
    }
}


// 删除
void delete_by_suburb_name(Suburb **head, const char *suburb_name, FILE *output_file) {
    Suburb *current = *head, *prev = NULL;
    int found = 0;
    int record_count = 0;

    while (current != NULL) {
        if (strcmp(current->official_name_suburb, suburb_name) == 0) {
            found = 1;
            record_count++;

            // 删除当前节点
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }

            Suburb *temp = current;
            current = current->next;

            // 释放内存
            free(temp->official_name_suburb);
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }

    // 输出
    if (record_count > 0) {
        fprintf(stdout, "%s --> %d records deleted\n", suburb_name, record_count);
    } else {
        fprintf(stdout, "%s --> NOTFOUND\n", suburb_name);
    }
}


// 释放链表
void free_data(Suburb *head) {
    Suburb *current = head;
    while (current != NULL) {
        Suburb *next = current->next;
        free(current->official_name_suburb);  // 释放动态分配的字符串
        free(current);  // 释放节点
        current = next;
    }
}