shell : main.c ls.c parse.c prompt.c
	gcc -o shell main.c parse.c ls.c prompt.c
