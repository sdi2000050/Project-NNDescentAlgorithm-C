LIBRARY_NAME = libgraph.a

all: graph_main

graph_main: graph_main.o $(LIBRARY_NAME)
	gcc graph_main.o -L. -lgraph -o graph_main -lm

$(LIBRARY_NAME): graph_Implementation.o nndescent_lj.o distances.o
	ar rcs $(LIBRARY_NAME) graph_Implementation.o nndescent_lj.o distances.o

graph_main.o: graph_main.c  
	gcc -c graph_main.c -o graph_main.o 

graph_Implementation.o: graph_Implementation.c 
	gcc -c graph_Implementation.c -o graph_Implementation.o 

nndescent_lj.o: nndescent_lj.c
	gcc -c nndescent_lj.c -o nndescent_lj.o

distances.o: distances.c
	gcc -c distances.c -o distances.o

test_results : test_results.o graph_Implementation.o nndescent_lj.o distances.o
	gcc graph_Implementation.o nndescent_lj.o distances.o test_results.o -o test_results -lm 

test_results.o : test_results.c 
	gcc -c test_results.c -o test_results.o 

test : test.o graph_Implementation.o nndescent_lj.o distances.o
	gcc graph_Implementation.o nn_descent.o distances.o test.o -o test -lm 

test.o : test.c 
	gcc -c test.c -o test.o 

clean:
	rm -f *.o *.a graph_main
	rm -f *.o test
	rm -f *.o test_results