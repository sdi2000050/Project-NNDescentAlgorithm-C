graph: graph.o graph_Implementation.o
	gcc graph_Implementation.o graph.o -o graph -lm

graph.o: graph.c  
	gcc -c graph.c -o graph.o 

graph_Implementation.o: graph_Implementation.c 
	gcc -c graph_Implementation.c -o graph_Implementation.o 

test : test.o graph_Implementation.o 
	gcc graph_Implementation.o test.o -o test -lm 

test.o : test.c 
	gcc -c test.c -o test.o 

clean:
	rm -f *.o graph
