dragonshell: dragonshell.cc compile
	g++ -Wall -std=c++11 dragonshell.o -o dragonshell

compile: dragonshell.cc
	g++ -c -Wall -std=c++11 dragonshell.cc

clean:
	rm -f dragonshell dragonshell.o

compress: dragonshell.cc readme.md
	tar -czf dragonshell.tar.gz readme.md dragonshell.cc Makefile
