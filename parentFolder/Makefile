CC = gcc

CFLAGS_DEBUG = -Wall -Wextra -std=c23 -pedantic -g -MMD -MP
CFLAGS_RELEASE = -Wall -Wextra -std=c23 -pedantic -O2 -MMD -MP

CFLAGS = $(CFLAGS_RELEASE)

SRC_DIR = src
OBJ_DIR = test

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

TARGET = parent

all: $(TARGET)

$(OBJ_DIR): 
	mkdir -p $(OBJ_DIR)

$(TARGET): $(OBJ) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

DEP = $(OBJ:.o=.d)
-include $(DEP)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

memcheck: debug 
	valgrind --leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	./$(TARGET)

debug: CFLAGS = $(CFLAGS_DEBUG)
debug: clean $(TARGET)

release: CFLAGS = $(CFLAGS_RELEASE)
release: clean $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean debug release run

