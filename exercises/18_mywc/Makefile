CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = 18_mywc
SRCS = 18_mywc.c mywc.c
OBJS := $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
