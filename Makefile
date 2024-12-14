CC = gcc
CFLAGS = -Iinc -Wall -Wextra -Wno-varargs -g 

SRC_DIR = ./src
INC_DIR = ./inc
OBJ_DIR = ./obj
TARGET = Dungeons_of_AYBU

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: clean build

build: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

execute:
	$(TARGET).exe

.PHONY: all