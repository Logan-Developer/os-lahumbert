FILENAME=pid
SOURCE=$(FILENAME).c
TARGET=$(FILENAME).out
CC=gcc
CCFLAGS=-Wall -Wextra -Werror -Wpedantic -std=c99 -g

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CCFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)