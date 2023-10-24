graph: graph.o graph_Implementation.o
	gcc graph_Implementation.o graph.o -o graph -lm

graph.o: graph.c  
	gcc -c graph.c -o graph.o 

graph_Implementation.o: graph_Implementation.c 
	gcc -c graph_Implementation.c -o graph_Implementation.o 

clean:
	rm -f *.o graph
