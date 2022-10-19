all: main.c
	gcc main.c -o main.out
	./main.out

clean: 
	rm -f main.out