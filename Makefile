CC = clang
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lm

SRC_DIR = ./src
BUILD_DIR = ./target

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
TARGET = $(BUILD_DIR)/hex2oklch

build: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: build
	$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
