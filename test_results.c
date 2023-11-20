#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graph.h"

Node** getknodes(Node** nodes, int numnodes, int dim, int k, Distancefunc distance_function);
KDistance** knodespoint(Node** nodes, int numnodes,int k, int stn, point* p, Distancefunc distance_function);

int main(int argc, char* argv[]){

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
    
    Node** knodes;
    
    Distancefunc dis; 
                                      
    if (strcmp(mode,"graph")==0){
        // Graph implementation of brute force
        //printf("Graph implementation:\n");
        if(strcmp(dist,"euclidean") == 0) {                                                          // and execute the algorithm according to dis_num imput!
            knodes = getknodes(nodes, numnodes, dim, k, euclidean_distance);       
        }
        else if(strcmp(dist,"manhattan") == 0) {
            knodes = getknodes(nodes, numnodes, dim, k, manhattan_distance);    
        }
        else if(strcmp(dist,"chebysev") == 0) {
            knodes = getknodes(nodes, numnodes, dim, k, chebyshev_distance);    
        }
        else{
            printf("Invalid distance\n");
            return 0;
        }

        //printf("Final KNN Graph:\n");
        for (int i = 0; i < numnodes; ++i) {
            Node* currentNode = knodes[i];
            //printf("Node %d kneighbors: ", currentNode->numnode);
            ListNode* kneighborNode = currentNode->kneighbors;
            while (kneighborNode != NULL) {
                printf("%d ", kneighborNode->node->numnode);
                kneighborNode = kneighborNode->nextnode;
            }
            printf("\n");
            free(kneighborNode);
        }
        for (int i = 0; i < numnodes; i++) {
            free(knodes[i]);
        }
        free(knodes);
    }else if (strcmp(mode,"point")==0){
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
            kd = knodespoint(nodes,numnodes,k,stn,p,euclidean_distance);       
        }
        else if(strcmp(dist,"manhattan") == 0) {
            kd =knodespoint(nodes,numnodes,k,stn,p,manhattan_distance);    
        }
        else if(strcmp(dist,"chebysev") == 0) {
            kd = knodespoint(nodes,numnodes,k,stn,p,chebyshev_distance);    
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
        free(kd);
        fclose(file);

    }

    free(nodes);

    return 0;
}

Node** getknodes(Node** nodes, int numnodes, int dim, int k, Distancefunc distance_function) {
    Node** knodes = (Node**)malloc(numnodes * sizeof(Node*));
    if (!knodes) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numnodes; i++) {
        knodes[i] = (Node*)malloc(sizeof(Node));
        if (!knodes[i]) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }

        knodes[i]->numnode = nodes[i]->numnode;
        knodes[i]->data = nodes[i]->data;
        knodes[i]->kneighbors = NULL;
        knodes[i]->rneighbors = NULL;

        KDistance** distances = (KDistance**)malloc((numnodes) * sizeof(KDistance*));
        if (!distances) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < numnodes; j++) {
                distances[j] = (KDistance*)malloc(sizeof(KDistance));
                if (!distances[j]) {
                    fprintf(stderr, "Memory allocation error\n");
                    exit(EXIT_FAILURE);
                }

                distances[j]->node = nodes[j];
                distances[j]->dis = distance_function(*(nodes[i]->data), *(nodes[j]->data));
            
        }

        sort(distances,numnodes);

        ListNode* currentNeighbor = NULL;
        for (int l = 0; l < k; l++) {
            ListNode* newNeighbor = (ListNode*)malloc(sizeof(ListNode));
            if (!newNeighbor) {
                fprintf(stderr, "Memory allocation error\n");
                exit(EXIT_FAILURE);
            }

            newNeighbor->node = distances[l+1]->node;
            newNeighbor->nextnode = NULL;

            if (!knodes[i]->kneighbors) {
                knodes[i]->kneighbors = newNeighbor;
                currentNeighbor = newNeighbor;
            } else {
                currentNeighbor->nextnode = newNeighbor;
                currentNeighbor = newNeighbor;
            }
        }

        for (int j = 0; j < numnodes-1; j++) {
            free(distances[j]);
        }
        free(distances);
    }

    return knodes;
}

KDistance** knodespoint(Node** nodes, int numnodes,int k, int stn, point* p, Distancefunc distance_function){
    KDistance** distances = (KDistance**)malloc((numnodes-stn) * sizeof(KDistance*));
    if (!distances) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numnodes-stn; i++) {
        distances[i] = (KDistance*)malloc(sizeof(KDistance));
        if (!distances[i]) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }

        distances[i]->node = nodes[i];
        distances[i]->dis = distance_function(*(nodes[i]->data),*p);
    }

    sort(distances,numnodes-stn);


    return distances;
}