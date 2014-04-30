# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SOURCES = instream.c spawn.c inject.c
ASM_SOURCES = code.s

OBJECTS = $(SOURCES:.c=.o) $(ASM_SOURCES:.s=.o)
TARGET = instream

.PHONY: all clean
all: $(TARGET)
clean:
	-rm $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

