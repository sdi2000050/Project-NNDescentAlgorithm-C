graph_main: graph_main.o graph_Implementation.o nn_descent.o
	gcc graph_Implementation.o nn_descent.o graph_main.o -o graph_main -lm

graph_main.o: graph_main.c  
	gcc -c graph_main.c -o graph_main.o 

graph_Implementation.o: graph_Implementation.c 
	gcc -c graph_Implementation.c -o graph_Implementation.o 

nn_descent.o: nn_descent.c
	gcc -c nn_descent.c -o nn_descent.o

test : test.o graph_Implementation.o nn_descent.o
	gcc graph_Implementation.o nn_descent.o test.o -o test -lm 

test.o : test.c 
	gcc -c test.c -o test.o 

clean:
	rm -f *.o graph_main
	rm -f *.o test
