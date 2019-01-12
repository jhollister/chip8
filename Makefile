CC = gcc

OBJS = *.c

FLAGS = -Werror -Wextra -std=c99

INCLUDE = -I. -Iinclude

EXE_NAME = chip8.exe

BUILD_DIR = bin

all: $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(OBJS) $(FLAGS) $(INCLUDE) -o $(BUILD_DIR)/$(EXE_NAME)

clean:
	rm -rf $(BUILD_DIR)/*