CC = gcc

ifneq (build, $(MAKECMDGOALS))
CFLAGS = -g -Wall -Wextra
endif

LDFLAGS = -lncurses -ltinfo

SRCS = $(filter-out main.c, $(wildcard *.c))
OBJS = $(SRCS:.c=.o)

PROG = pong

all: $(PROG)

build: clean $(PROG)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCS)
	$(CC) -MM $^ > $@

$(PROG): main.c $(OBJS)
	$(CC) -static $^ $(LDFLAGS) -o $@

clean:
	rm -rf *.o $(PROG) deps.mk

.PHONY = all clean
