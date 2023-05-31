.PHONY: clean
main: main.c
	gcc main.c -o main -lm
debug: main.c
	gcc -g main.c -o main -lm
clean:
	-rm -v main
