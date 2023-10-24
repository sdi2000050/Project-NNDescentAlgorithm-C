#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "graph.h"

int main(int argc, char *argv[]) {

    char *input_file = NULL;
    int numnodes = 0;
    int dim = 0;
    int k = 0;

    if (argc != 5) {
        fprintf(stderr, "Usage: %s input_file numnodes dimension k\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    input_file = argv[1];
    numnodes = atoi(argv[2]);
    dim = atoi(argv[3]);
    k = atoi(argv[4]); 

    if (numnodes <= 0 || dim <= 0 || k<=0) {
        fprintf(stderr, "Invalid number of nodes or dimension.\n");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(input_file, "r");                    // Open and process the input file 
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", input_file);
        exit(EXIT_FAILURE);
    }
    
    Graph* graph = createGraph(numnodes);
    Node** nodes = getnodes(file,numnodes,dim);

    addEdge(graph, nodes[0], nodes[1]);
    addEdge(graph, nodes[0], nodes[2]);
    addEdge(graph, nodes[1], nodes[3]);
    addEdge(graph, nodes[2], nodes[5]);
    addEdge(graph, nodes[3], nodes[5]);
    addEdge(graph, nodes[3], nodes[1]);
    addEdge(graph, nodes[4], nodes[2]);
    addEdge(graph, nodes[4], nodes[0]);
    addEdge(graph, nodes[5], nodes[1]);


    printNeighbors(graph);

    free(nodes);
    free(graph->nodes);
    free(graph);

    return 0;
}