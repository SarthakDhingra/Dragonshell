dragonshell: handler.o dragonshell.o
	gcc -Wall -std=c99 -g -o dragonshell handler.o dragonshell.o
drgaonshell.o: dragonshell.oc
	gcc -Wall -std=c99 -g -c dragonshell.c
handler.o: handler.c
	gcc -Wall -std=c99 -g -c handler.c
clean:
	rm dragonshell
	rm -f test *.o


	#c flags
