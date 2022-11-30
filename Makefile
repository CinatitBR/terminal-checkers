all: main.cpp
	g++ main.cpp -o main.out
	./main.out

clean: 
	rm -f main.out