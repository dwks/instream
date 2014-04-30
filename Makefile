# Makefile
CC = gcc
CFLAGS = -Wall -Wextra

SOURCES = instream.c spawn.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = instream

.PHONY: all clean
all: $(TARGET)
clean:
	-rm $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

