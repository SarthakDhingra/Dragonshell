#the compiler
CC = g++

#compiler flags:
# -g adds debugging info to exe
# -Wall turnos on most compiler warning
CFLAGS = -g -Wall -std=c++11

OBJECTS = dragonshell.o handler.o

dragonshell: $(OBJECTS)
	$(CC) -o dragonshell $(OBJECTS)
dragonshell.o: dragonshell.cc handler.h
	$(CC) $(CFLAGS) -c dragonshell.cc -o dragonshell.o
handler.o: handler.cc handler.h
	$(CC) $(CFLAGS) -c handler.cc -o handler.o
clean:
	rm dragonshell
	rm *.o 
