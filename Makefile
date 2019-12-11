all: sum shell

sum: sum.c
	gcc sum.c -lpthread -o sum 

shell: shell.cc
	g++ -o shell shell.cc