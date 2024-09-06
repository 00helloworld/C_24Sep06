#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s 1 <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    // 读取数据
    Suburb *head = load_data(argv[2]);

    FILE *output_file = fopen(argv[3], "w");

    // 读取查询
    char suburb_name[128];
    while (fgets(suburb_name, sizeof(suburb_name), stdin)) {
        // 移除换行符
        suburb_name[strcspn(suburb_name, "\n")] = 0;
        search_by_suburb_name(head, suburb_name, output_file);
    }

    fclose(output_file);
    return 0;
}
