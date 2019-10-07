CC = g++

CFLAGS = -Wall -std=c++11

OBJECTS = dragonshell.o handler.o

	#c flags
dragonshell: $(OBJECTS)
	$(CC) -o dragonshell $(OBJECTS)
dragonshell.o: dragonshell.cc handler.h
	$(CC) $(CFLAGS) -c dragonshell.cc -o dragonshell.o
handler.o: handler.cc handler.h
	$(CC) $(CFLAGS) -c handler.cc -o handler.o
clean:
	rm *.o
	rm dragonshell
