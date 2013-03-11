#!/bin/bash

CC = gcc
files = main.c.c functions.c pipe.c
objects = main.o functions.o pipe.o
LIBS = -lm -pthread
OUT = pipeline

CFLAGS = -W -Wall -Wextra

.PHONY: all
all: $(OUT)
	@echo "Pipeline Compiled..."

$(OUT): $(objects)
	$(CC) $(CFLAGS) $(objects) $(LIBS) -o $@

main.o: main.c globals.h functions.h pipe.h
	$(CC) -c -g main.c

functions.o: functions.c functions.h globals.h
	$(CC) -c -g functions.c

pipe.o: pipe.c pipe.h functions.h globals.h
	$(CC) -c -g pipe.c


.PHONY: clean
clean :
	rm -f *.o *.out *~
	@echo "Object files Deleted..."
