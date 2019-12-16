all: sum shell pipe

sum: sum.c
	gcc sum.c -lpthread -o sum 

shell: shell.cc
	g++ -o shell shell.cc

pipe: pipe.c
	gcc pipe.c -o pipe 
