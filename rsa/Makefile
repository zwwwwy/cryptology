CC      = gcc
CFLAGS  = -g

SRC_DIR = .
OBJ_DIR = ./build/objs
BUILD_DIR = ./build

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
UTIL_FILES = $(OBJ_DIR)/utils.o

all: $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

key: $(OBJ_DIR)/key_generator.o
	$(CC) $(CFLAGS) $(OBJ_DIR)/key_generator.o -o $(BUILD_DIR)/key_generator.out -lm
	$(BUILD_DIR)/key_generator.out

test: $(OBJ_DIR)/test.o $(UTIL_FILES)
	$(CC) $(CFLAGS) $(OBJ_DIR)/test.o $(UTIL_FILES) -o $(BUILD_DIR)/test.out
	$(BUILD_DIR)/test.out


clean:
	rm -f $(OBJ_FILES) $(BUILD_DIR)/*.out

h:
	@echo des
	@echo str
	@echo tri
	@echo diff


