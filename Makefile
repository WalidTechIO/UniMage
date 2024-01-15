CC = gcc

GDK_PIXBUF_PATH = $(shell pkg-config --variable=libdir gdk-pixbuf-2.0)/gdk-pixbuf-2.0

SRC_DIR = src
BUILD_DIR = build
INCLUDES_DIR = include
TEST_DIR = tests

CFLAGS = -Wall -g $(addprefix -I, $(INCLUDES_DIR)) $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = -lm $(shell pkg-config --libs gtk+-3.0)

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.c=.o)))
TEST_OBJS = $(filter-out $(BUILD_DIR)/main.o,$(OBJS))

TEST_EXECUTABLE=$(TEST_DIR)/test.exe
EXECUTABLEW = $(BUILD_DIR)/bin/executable.exe
EXECUTABLEL = $(BUILD_DIR)/bin/program

.PHONY: all run debug clean test

OS_DETECTE=$(shell uname)

ifneq (,$(findstring MSYS_NT,$(OS_DETECTE)))
	EXECUTABLE = $(EXECUTABLEW)
	OS_DETERMINE = Windows
else
	EXECUTABLE = $(EXECUTABLEL)
	OS_DETERMINE = Linux
endif

all: $(EXECUTABLE)

$(EXECUTABLEL): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(EXECUTABLEW): $(OBJS) $(BUILD_DIR)/icon.o
	$(CC) $^ $(BUILD_DIR)/icon.o -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/icon.o: icon.ico $(SRC_DIR)/icon.rc
	windres $(SRC_DIR)/icon.rc $@

run: $(EXECUTABLE)
	$(EXECUTABLE)

debug: $(EXECUTABLE)
	gdb $(EXECUTABLE)

clean:
	rm -f $(wildcard $(BUILD_DIR)/*.o) $(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_DIR)/test.c $(TEST_OBJS)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

test: $(TEST_EXECUTABLE)
	$(TEST_EXECUTABLE)

config:
	mkdir $(BUILD_DIR)
	mkdir $(BUILD_DIR)/bin
	cp ui.glade $(BUILD_DIR)/bin/ui.glade
ifeq ($(OS_DETERMINE),Windows)
	mkdir $(BUILD_DIR)/lib
	mkdir $(BUILD_DIR)/share
	mkdir $(BUILD_DIR)/share/glib-2.0
	cp -r $(GDK_PIXBUF_PATH) $(BUILD_DIR)/lib/gdk-pixbuf-2.0
	cp -r /mingw32/share/icons $(BUILD_DIR)/share/icons
	cp -r /mingw32/share/glib-2.0/schemas $(BUILD_DIR)/share/glib-2.0/schemas
	glib-compile-schemas $(BUILD_DIR)/share/glib-2.0/schemas/
endif