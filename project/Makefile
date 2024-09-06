# 编译器和编译选项
CC = gcc
CFLAGS = -Wall -Iinclude -g

# dict1 的源文件和对象文件
SRC1 = src/dict1.c src/dict.c
OBJ1 = $(SRC1:.c=.o)
EXE1 = dict1

# dict2 的源文件和对象文件
SRC2 = src/dict2.c src/dict.c
OBJ2 = $(SRC2:.c=.o)
EXE2 = dict2

# dict3 的源文件和对象文件
SRC3 = src/dict3.c src/dict.c
OBJ3 = $(SRC3:.c=.o)
EXE3 = dict3

# dict4 的源文件和对象文件
SRC4 = src/dict4.c src/dict.c src/patricia_tree.c
OBJ4 = $(SRC4:.c=.o)
EXE4 = dict4

# 默认目标，编译所有字典程序
all: $(EXE1) $(EXE2) $(EXE3) $(EXE4)

# 编译 dict1 的规则
$(EXE1): $(OBJ1)
	$(CC) $(CFLAGS) -o $(EXE1) $(OBJ1)

# 编译 dict2 的规则
$(EXE2): $(OBJ2)
	$(CC) $(CFLAGS) -o $(EXE2) $(OBJ2)

# 编译 dict3 的规则
$(EXE3): $(OBJ3)
	$(CC) $(CFLAGS) -o $(EXE3) $(OBJ3)

# 编译 dict4 的规则
$(EXE4): $(OBJ4)
	$(CC) $(CFLAGS) -o $(EXE4) $(OBJ4)

# 将 .c 文件编译为 .o 文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理所有编译生成的文件
clean:
	rm -f $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(EXE1) $(EXE2) $(EXE3) $(EXE4)

# 运行 dict1 的规则
run1: $(EXE1)
	@read -p "Enter input CSV file: " input_csv; \
	read -p "Enter output file: " output_file; \
	./$(EXE1) 1 $$input_csv $$output_file < tests/test1.in

# 运行 dict2 的规则
run2: $(EXE2)
	@read -p "Enter input CSV file: " input_csv; \
	read -p "Enter output file: " output_file; \
	./$(EXE2) 2 $$input_csv $$output_file < tests/test2.in

# 运行 dict3 的规则
run3: $(EXE3)
	@read -p "Enter input CSV file: " input_csv; \
	read -p "Enter output file: " output_file; \
	./$(EXE3) 3 $$input_csv $$output_file < tests/test3.in

# 运行 dict4 的规则
run4: $(EXE4)
	@read -p "Enter input CSV file: " input_csv; \
	read -p "Enter output file: " output_file; \
	./$(EXE4) 4 $$input_csv $$output_file < tests/test4.in
