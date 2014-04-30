# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Idistorm3/include
LDFLAGS = distorm3/distorm3.a

SOURCES = instream.c spawn.c inject.c dump.c
ASM_SOURCES = code.s

OBJECTS = $(SOURCES:.c=.o) $(ASM_SOURCES:.s=.o)
TARGET = instream

.PHONY: all clean reallyclean
all: distorm3.built $(TARGET)
distorm3.built:
	if [ ! -d distorm3 ]; then unzip distorm3.zip; fi
	$(MAKE) -C distorm3/make/linux
	touch distorm3.built
clean:
	-rm $(TARGET) $(OBJECTS)
reallyclean: clean
	-rm -rf distorm3 distorm3.built

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

