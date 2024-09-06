#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"
#include "search_result.h"

// 处理引号和逗号
char *parse_field_(char **line) {
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

// 加载数据并插入到 Patricia Tree 中
PatriciaNode *load_data_to_patricia(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file %s\n", filename);
        return NULL;
    }

    PatriciaNode *root = NULL;

    // 忽略表头
    char buffer[512];
    fgets(buffer, sizeof(buffer), file);

    // 读取每一行并插入到 Patricia Tree
    while (fgets(buffer, sizeof(buffer), file)) {
        Suburb *new_node = (Suburb *)malloc(sizeof(Suburb));

        char *line = buffer;

        new_node->comp20003_code = atoi(parse_field_(&line));
        new_node->official_code_suburb = atoi(parse_field_(&line));
        new_node->official_name_suburb = parse_field_(&line);
        new_node->year = atoi(parse_field_(&line));
        new_node->official_code_state = parse_field_(&line);
        new_node->official_name_state = parse_field_(&line);
        new_node->official_code_local_government_area = parse_field_(&line);
        new_node->official_name_local_government_area = parse_field_(&line);
        new_node->latitude = atof(parse_field_(&line));
        new_node->longitude = atof(parse_field_(&line));

        // 将新节点插入到 Patricia Tree
        root = insert_patricia(root, new_node->official_name_suburb, new_node);
    }

    fclose(file);
    return root;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <stage> <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    // 获取阶段、输入文件和输出文件
    FILE *output_file = fopen(argv[3], "w");
    if (!output_file) {
        printf("Could not open output file %s\n", argv[3]);
        return 1;
    }

    // 加载数据到 Patricia 树
    PatriciaNode *root = load_data_to_patricia(argv[2]);

    char query[128];
    SearchResult result;

    // 从标准输入读取查询
    while (fgets(query, sizeof(query), stdin)) {
        query[strcspn(query, "\n")] = 0;  // 去掉换行符

        // 根据 stage 进行处理
        result = fuzzy_search_patricia(root, query);

        // 输出suburb name
        fprintf(output_file, "%s -->\n", query);

        if (result.found > 0) {
            // 如果找到匹配项，无论是精确匹配还是模糊匹配
            Suburb *suburb = result.closest_suburb;  // 通过 result 获取匹配到的 Suburb

            // 输出匹配的 Suburb 信息
            fprintf(output_file, "COMP20003 Code: %d,Official Code Suburb: %d,Official Name Suburb: %s,Year: %d ", 
                suburb->comp20003_code,
                suburb->official_code_suburb,
                suburb->official_name_suburb,
                suburb->year);

            // 检查每个字段是否包含逗号，并使用双引号包裹
            if (strchr(suburb->official_code_state, ',') != NULL) {
                fprintf(output_file, ",Official Code State: \"%s\"", suburb->official_code_state);
            } else {
                fprintf(output_file, ",Official Code State: %s", suburb->official_code_state);
            }
            if (strchr(suburb->official_name_state, ',') != NULL) {
                fprintf(output_file, ",Official Name State: \"%s\"", suburb->official_name_state);
            } else {
                fprintf(output_file, ",Official Name State: %s", suburb->official_name_state);
            }
            if (strchr(suburb->official_code_local_government_area, ',') != NULL) {
                fprintf(output_file, ",Official Code Local Government Area: \"%s\"", suburb->official_code_local_government_area);
            } else {
                fprintf(output_file, ",Official Code Local Government Area: %s", suburb->official_code_local_government_area);
            }
            if (strchr(suburb->official_name_local_government_area, ',') != NULL) {
                fprintf(output_file, ",Official Name Local Government Area: \"%s\"", suburb->official_name_local_government_area);
            } else {
                fprintf(output_file, ",Official Name Local Government Area: %s", suburb->official_name_local_government_area);
            }

            // 输出经纬度信息
            fprintf(output_file, ",Latitude: %lf,Longitude: %lf\n", 
                suburb->latitude,
                suburb->longitude);

            // 在终端中输出比较信息和记录
            printf("%s --> %d records found - comparisons: b%d n%d s%d\n", 
                query, 
                result.found, 
                result.bit_comparisons, 
                result.node_accesses, 
                result.string_comparisons);

        } else {
            printf("%s --> NOTFOUND\n", query);
        }

    }

    // 关闭文件和释放树
    fclose(output_file);
    free_patricia(root);

    return 0;
}
