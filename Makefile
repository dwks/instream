# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Idistorm3/include
LDFLAGS = distorm3/distorm3.a

SOURCES = instream.c spawn.c inject.c dump.c
ASM_SOURCES = code.s

OBJECTS = $(SOURCES:.c=.o) $(ASM_SOURCES:.s=.o)
TARGET = instream

.PHONY: all clean
all: $(TARGET)
clean:
	-rm $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

