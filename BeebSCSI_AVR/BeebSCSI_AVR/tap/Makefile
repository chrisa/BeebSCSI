TARGET  = prove

CC      = cc
CFLAGS  = -O1 -Wall -std=c99 -I .

SOURCES = $(wildcard *.c)

OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^

clean:
	rm -f $(OBJECTS) $(TARGET)

