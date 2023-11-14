CC = gcc
CFLAGS = -Wall -g -Iinclude $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = -lm $(shell pkg-config --libs gtk+-3.0)

SRC_DIR = src
BUILD_DIR = build

BMP_FILES = $(wildcard *.bmp)
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.c=.o)))
EXECUTABLE = $(BUILD_DIR)/executable

.PHONY: all run debug clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) build/stack2.o $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXECUTABLE)
	$(EXECUTABLE) # > run.log

debug: $(EXECUTABLE)
	gdb $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) $(OBJS) $(BMP_FILES) run.log

(BUILD_DIR)/%.o: include/%.h