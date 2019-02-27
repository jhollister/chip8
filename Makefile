CC = gcc

OBJS = src/*.c

FLAGS = -Werror -Wpedantic -std=c99

INCLUDE = -I. -Iinclude -IC:/MinGW/include

LIB_DIR = -LC:/MinGW/lib

SDL_LIBS = -lmingw32 -lSDL2main -lSDL2

EXE_NAME = chip8.exe

BUILD_DIR = bin

all: $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(OBJS) $(INCLUDE) $(LIB_DIR) $(SDL_LIBS) $(FLAGS) -o $(BUILD_DIR)/$(EXE_NAME)

clean:
	rm -rf $(BUILD_DIR)