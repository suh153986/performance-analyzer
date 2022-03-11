CC := g++
CFLAGS := -Wall -g
TARGET := example

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

run: all
	./$(TARGET)

clean:
	rm -rf $(TARGET) *.o
