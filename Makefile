CC = gcc
CFLAGS = -Wall -Wextra -std=c11 $(shell pkg-config --cflags sdl2 SDL2_image)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image)
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = build/renderer

$(TARGET): $(OBJ) res
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: res
res:
	@mkdir -p build/res
	cp -r res/* build/res/

clean:
	rm -rf build

run: $(TARGET)
	cd build && ./renderer

.PHONY: clean run
