FILENAME=fork
SOURCE=$(FILENAME).c
TARGET=$(FILENAME).out
CC=gcc

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(SOURCE)
	$(CC) -o $@ $<

clean:
	rm -f $(TARGET)