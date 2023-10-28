#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "graph.h"

int main(int argc, char *argv[]) {

    char *input_file = NULL;
    int numnodes;
    int dim = 0;
    int k = 0;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s input_file dimension k\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    input_file = argv[1];
    dim = atoi(argv[2]);
    k = atoi(argv[3]); 

    if (dim <= 0 || k<=0) {
        fprintf(stderr, "Invalid number of nodes or dimension.\n");
        exit(EXIT_FAILURE);
    }

    Node** nodes = getnodes(input_file,&numnodes,dim);

    Graph* graph = createGraph(numnodes);
    

    createRandomGraph (graph,nodes,k);

    printNeighbors(graph);


    free(nodes);
    free(graph->nodes);
    free(graph);

    return 0;
}