graph_main: graph_main.o graph_Implementation.o
	gcc graph_Implementation.o graph_main.o -o graph_main -lm

graph_main.o: graph_main.c  
	gcc -c graph_main.c -o graph_main.o 

graph_Implementation.o: graph_Implementation.c 
	gcc -c graph_Implementation.c -o graph_Implementation.o 

clean:
	rm -f *.o graph_main
