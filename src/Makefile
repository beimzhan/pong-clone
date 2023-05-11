CC = gcc

ifeq (build, $(MAKECMDGOALS))
CFLAGS = -static
LDFLAGS = -lncurses -ltinfo
else
LDFLAGS = -lncurses
ifneq (macos, $(MAKECMDGOALS))
CFLAGS = -g -Wall -Wextra
endif
endif

SRCS = $(filter-out main.c, $(wildcard *.c))
OBJS = $(SRCS:.c=.o)

PROG = pong

all: $(PROG)

build: clean $(PROG)

macos: clean $(PROG)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCS)
	$(CC) -MM $^ > $@

$(PROG): main.c $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -rf *.o $(PROG) deps.mk

.PHONY = all build clean
