# Make Seebot Services

CC=gcc
EXECUTABLE=services
OBJECTS=irc.o functions.o main.o
FLAGS=-Wall

all: main

main: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(FLAGS)
	
clean:
	rm *o $(EXECUTABLE)
