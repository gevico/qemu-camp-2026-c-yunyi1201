CC = gcc
CFLAGS = -Wall -g -O2


INCLUDEDIR := .

TARGET = 19_mytrans
SRCS = 19_mytrans.c mytrans.c myhash.c
OBJS := $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
