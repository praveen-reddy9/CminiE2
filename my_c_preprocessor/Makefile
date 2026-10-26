CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic

TARGET = cpre
OBJECTS = main.o preprocessor.o comment.o macro.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

main.o: main.c preprocessor.h
	$(CC) $(CFLAGS) -c main.c

preprocessor.o: preprocessor.c preprocessor.h comment.h macro.h
	$(CC) $(CFLAGS) -c preprocessor.c

comment.o: comment.c comment.h
	$(CC) $(CFLAGS) -c comment.c

macro.o: macro.c macro.h
	$(CC) $(CFLAGS) -c macro.c

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean
