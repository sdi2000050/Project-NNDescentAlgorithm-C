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
    /*// Print the coordinates and their dimentions
    for (int i = 0; i < numnodes; ++i) {
        printf("Point %u (Dimension %u): ", i, nodes[i]->data->dim);
        for (int j = 0; j < nodes[i]->data->dim; ++j) {
            printf("%.8f ", nodes[i]->data->coord[j]);
        }
        printf("\n");
    }*/

    Graph* graph = createGraph(numnodes);
    

    createRandomGraph (graph,nodes,k);


    printNeighbors(graph);

    free(nodes);
    free(graph->nodes);
    free(graph);

    return 0;
}