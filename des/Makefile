CC      = gcc
CFLAGS  = -g

SRC_DIR = .
OBJ_DIR = ./build/objs
BUILD_DIR = ./build

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

DES_BASE = $(OBJ_DIR)/des.o $(OBJ_DIR)/replacement.o $(OBJ_DIR)/odd_check.o $(OBJ_DIR)/tables.o

all: $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

des: $(DES_BASE) $(OBJ_DIR)/des_test.o
	$(CC) $(CFLAGS) $(DES_BASE) $(OBJ_DIR)/des_test.o -o $(BUILD_DIR)/des.out
	$(BUILD_DIR)/des.out

str: $(DES_BASE) $(OBJ_DIR)/des_str.o
	$(CC) $(CFLAGS) $(DES_BASE) $(OBJ_DIR)/des_str.o -o $(BUILD_DIR)/str.out
	$(BUILD_DIR)/str.out

tri: $(DES_BASE) $(OBJ_DIR)/des_tri.o
	$(CC) $(CFLAGS) $(DES_BASE) $(OBJ_DIR)/des_tri.o -o $(BUILD_DIR)/tri.out
	$(BUILD_DIR)/tri.out

diff: $(DES_BASE) $(OBJ_DIR)/list.o $(OBJ_DIR)/diff_attack.o
	$(CC) $(CFLAGS) $(DES_BASE) $(OBJ_DIR)/list.o $(OBJ_DIR)/diff_attack.o -o $(BUILD_DIR)/diff_attack.out
	$(BUILD_DIR)/diff_attack.out

clean:
	rm -f $(OBJ_FILES) $(BUILD_DIR)/*.out

h:
	@echo des
	@echo str
	@echo tri
	@echo diff


