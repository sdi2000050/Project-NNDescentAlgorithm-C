#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
    printf("Initial Graph:\n");
    printNeighbors(graph);
    printf("\n");

    Distancefunc dis;
    printNeighbors(graph);
    printf("Press 1 for euclidean distance\nPress 2 for manhattan distance\nPress 3 for chebyshev distance \n");
    int dis_num;
    scanf("%d", &dis_num);
    if(dis_num == 1) {
        nndescent(graph,k,euclidean_distance);    
    }
    if(dis_num == 2) {
        nndescent(graph,k,manhattan_distance);    
    }
    if(dis_num == 3) {
        nndescent(graph,k,chebyshev_distance);    
    }


    printf("Final KNN Graph:\n");
    
    printNeighbors(graph);

    free(nodes);
    free(graph->nodes);
    free(graph);

    return 0;
}