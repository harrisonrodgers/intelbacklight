CC = gcc
CFLAGS = -W -Wall -pedantic -std=c11
TARGET = intelbacklight

all: $(TARGET).c
	$(CC) -o $(TARGET) $(TARGET).c $(CFLAGS)

clean:
	rm -rf *o $(TARGET)
