LIBRARY_NAME = libgraph.a

all: graph_main

graph_main: graph_main.o $(LIBRARY_NAME)
	gcc graph_main.o -L. -lgraph -o graph_main -lm

$(LIBRARY_NAME): graph_Implementation.o nn_descent.o distances.o
	ar rcs $(LIBRARY_NAME) graph_Implementation.o nn_descent.o distances.o

graph_main.o: graph_main.c  
	gcc -c graph_main.c -o graph_main.o 

graph_Implementation.o: graph_Implementation.c 
	gcc -c graph_Implementation.c -o graph_Implementation.o 

nn_descent.o: nn_descent.c
	gcc -c nn_descent.c -o nn_descent.o

distances.o: distances.c
	gcc -c distances.c -o distances.o

test : test.o graph_Implementation.o nn_descent.o
	gcc graph_Implementation.o nn_descent.o test.o -o test -lm 

test.o : test.c 
	gcc -c test.c -o test.o 

clean:
	rm -f *.o *.a graph_main
	rm -f *.o test
