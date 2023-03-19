CC = gcc

CFLAGS = -g -Wall -Wextra -ansi -pedantic
LDFLAGS = -lncurses

SRCS = $(filter-out main.c, $(wildcard *.c))
OBJS = $(SRCS:.c=.o)

PROG = pong

all: $(PROG)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCS)
	$(CC) -MM $^ > $@

$(PROG): main.c $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

clean:
	rm -rf *.o $(PROG) deps.mk

.PHONY = all clean
