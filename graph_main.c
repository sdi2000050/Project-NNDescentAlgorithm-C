#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graph.h"

int main(int argc, char *argv[]) {

    char *input_file = NULL;
    char *input_point = NULL;
    int numnodes;
    int dim = 0;
    int k = 0;

    if (argc != 5) {
        fprintf(stderr, "Usage: %s input_file file_point dimension k\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    input_file = argv[1];
    input_point = argv[2];
    dim = atoi(argv[3]);
    k = atoi(argv[4]); 

    if (dim <= 0 || k <= 0) {
        fprintf(stderr, "Invalid number of nodes or dimension.\n");
        exit(EXIT_FAILURE);
    }

    Node** nodes = getnodes(input_file,&numnodes,dim);      // Get the nodes from the file

    Graph* graph = createGraph(numnodes);
    

    createRandomGraph (graph,nodes,k);                      // and create a random graph
    printf("Initial Graph:\n");
    printNeighbors(graph);
    printf("\n");

    Distancefunc dis;                                       
    printf("Press 1 for euclidean distance\nPress 2 for manhattan distance\nPress 3 for chebyshev distance \n");
    int dis_num;
    scanf("%d", &dis_num);                                  // Selection of the distance function from the 3 available options!
    int nn_num;
    printf("Press 1 for nn descent algorithm for the whole graph\nPress 2 for nn descent algorithm for one point\n");
    scanf("%d", &nn_num);                                   // Select the format of the algorithm!
    if(nn_num == 1){                                        
        if(dis_num == 1) {                                  //execute the algorithm according to dis_num imput!
            nndescent(graph,k,euclidean_distance);    
        }
        else if(dis_num == 2) {
            nndescent(graph,k,manhattan_distance);    
        }
        else if(dis_num == 3) {
            nndescent(graph,k,chebyshev_distance);    
        }
        else{
            printf("Invalid input\n");
        }
        
        printf("Final KNN Graph:\n");
        
        printNeighbors(graph);
    }
    else if(nn_num == 2){
        point* p = malloc(sizeof(point));
        FILE* file = fopen(input_point,"rb");               // Get the coordinates of the point from the file
        if (file == NULL) {
            fprintf(stderr, "Error opening file: %s\n", input_point);
            exit(EXIT_FAILURE);
        }
        
        if (p == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }

        float* values = malloc(dim * sizeof(float));
        if (values == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }

        
        if (fread(values, sizeof(float), dim, file) != dim) {         // Read the floats coords for the current point from the file
            fprintf(stderr, "Error reading data from file\n");
            exit(EXIT_FAILURE);
        }

        setcoords(p,values,dim);                                        //Set the coordinates of the point
        printf("Give starting node out of %d\n",numnodes);
        int n;
        scanf("%d",&n);
        KDistance** kd;
        if(dis_num == 1) {                                              // and execute the algorithm according to dis_num imput!
            kd = nndescentpoint(graph,k,n,*p,euclidean_distance);       
        }
        else if(dis_num == 2) {
            kd =nndescentpoint(graph,k,n,*p,manhattan_distance);    
        }
        else if(dis_num == 3) {
            kd = nndescentpoint(graph,k,n,*p,chebyshev_distance);    
        }
        else{
            printf("Invalid input\n");
        }
        printf("%d nearest nodes to point:\n",k);
        for(int i=0; i < k; i++){
            printf("%d ",kd[i]->node->numnode);
        }
        printf("\n");


        free(p->coord); 
        free(p);        

        free(values);  
        for(int i=0; i<k; i++){
            free(kd[i]);
        }
        free(kd);
        fclose(file);

    }
    else {
        printf("Invalid imput of algorithm format");
    }
    

    for(int i=0; i<numnodes; i++){
        free(nodes[i]);
    }
    free(nodes);
    free(graph->nodes);
    free(graph);

    return 0;
}