#!/bin/bash

BINDIR = ./bin
SRCDIR = ./src
AUXDIR = ./aux

CC = gcc
CFLAGS = -std=c99 -Wall -Werror -g -O0
LDFLAGS = -lm

all: montador-p1.x montador-p2.x

montador-p1.x:
	cp $(AUXDIR)/main1.c $(SRCDIR)/main.c
	$(CC) $(CFLAGS) -o $(BINDIR)/montador-p1.x $(SRCDIR)/*.c $(LDFLAGS)

montador-p2.x:
	cp $(AUXDIR)/main2.c $(SRCDIR)/main.c
	$(CC) $(CFLAGS) -o $(BINDIR)/montador-p2.x $(SRCDIR)/*.c $(LDFLAGS)

clean:
	$(RM) $(BINDIR)/*.x
	$(RM) $(SRCDIR)/*.o
