FILENAME=probDice
SOURCE=$(FILENAME).cpp
TARGET=$(FILENAME).out
CC=g++

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(SOURCE)
	$(CC) -o $@ $<

clean:
	rm -f $(TARGET)