CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = Phys

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
