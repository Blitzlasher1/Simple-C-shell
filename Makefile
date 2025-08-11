# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

SRCS = main.c parser.c executor.c
OBJS = $(SRCS:.c=.o)

TARGET = simple_shell

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)