# Make Seebot

CC=gcc
EXECUTABLE=bot
OBJECTS=irc.o functions.o main.o
FLAGS=-Wall

all: main

main: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(FLAGS)
	
clean:
	rm *o bot
