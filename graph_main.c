#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graph.h"

int main(int argc, char *argv[]) {

    char *input_file = NULL;
    char *input_point = NULL;
    int stn;
    int numnodes;
    int dim = 0;
    int k = 0;
    char* dist;
    char* mode;

    if (argc != 8) {
        fprintf(stderr, "Usage: %s input_file file_point starting_node dimension k distance mode\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    input_file = argv[1];
    input_point = argv[2];
    stn = atoi(argv[3]);
    dim = atoi(argv[4]);
    k = atoi(argv[5]); 
    dist = argv[6];
    mode = argv[7];

    if (dim <= 0 || k <= 0) {
        fprintf(stderr, "Invalid number of nodes or dimension.\n");
        exit(EXIT_FAILURE);
    }

    Node** nodes = getnodes(input_file,&numnodes,dim);      // Get the nodes from the file

    Graph* graph = createGraph(numnodes);
    

    createRandomGraph (graph,nodes,k);                      // And create a random graph
    printf("Initial Graph:\n");

    //printNeighbors(graph);
    printf("\n");

    Distancefunc dis;                                       
    /*if (strcmp(mode,"point")==0){
        // Point implementation of NN Descent   
        printf("\nPoint implementation:\n"); 
        point* p = malloc(sizeof(point));
        FILE* file = fopen(input_point,"rb");                       // Get the coordinates of the point from the file
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

        
        if (fread(values, sizeof(float), dim, file) != dim) {           // Read the floats coords for the current point from the file
            fprintf(stderr, "Error reading data from file\n");
            exit(EXIT_FAILURE);
        }

        setcoords(p,values,dim);                                        //Set the coordinates of the point
        
        KDistance** kd;
        if(strcmp(dist,"euclidean") == 0) {                                                          // and execute the algorithm according to dis_num imput!
            kd = nndescentpoint(graph,k,stn,*p,euclidean_distance);       
        }
        else if(strcmp(dist,"manhattan") == 0) {
            kd =nndescentpoint(graph,k,stn,*p,manhattan_distance);    
        }
        else if(strcmp(dist,"chebysev") == 0) {
            kd = nndescentpoint(graph,k,stn,*p,chebyshev_distance);    
        }
        else{
            printf("Invalid distance\n");
            return 0;
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

    }else */if (strcmp(mode,"graph")==0){
        // Graph implementation NN Descent 
        printf("\nGraph implementation:\n");
        if(strcmp(dist,"euclidean") == 0) {                                  // Take distance
            initialize_arrays(graph,k,euclidean_distance);
            local_join(graph,k,euclidean_distance);    
        }
        else if(strcmp(dist,"manhattan") == 0) {
            initialize_arrays(graph,k,manhattan_distance);    
            local_join(graph,k,manhattan_distance);
        }
        else if(strcmp(dist,"chebysev") == 0) {
            initialize_arrays(graph,k,chebyshev_distance);    
            local_join(graph,k,chebyshev_distance);
        }
        else{
            printf("Invalid distance\n");
            return 0;
        }
        printf("Final KNN Graph:\n");
        
        printNeighbors(graph);
    }
    

    for(int i=0; i<numnodes; i++){
        free(nodes[i]);
    }
    free(nodes);
    free(graph->nodes);
    free(graph);

    return 0;
}