#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "graph.h"


void setcoords(point *p, double coords[], int dimention) {
    p->dim = dimention;
    p->coord = (double*)malloc(dimention * sizeof(double));

    for(int i = 0; i < dimention; i++) {
        p->coord[i] = coords[i];
    }
}

void printpoint(point point) {
    for(int i = 0; i < point.dim ; i++) {
        printf("%f ", point.coord[i]);
    }
    printf("\n");
}

Node* create_node(int n, point* p) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->numnode = n;
    node->data = p;
    node->kneighbors = NULL;
    node->rneighbors = NULL;
    return node;
}

Graph* createGraph(int numnodes) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numnodes = numnodes;
    graph->nodes = (Node**)malloc(numnodes * sizeof(Node*));

    for (int i = 0; i < numnodes; ++i) {
        graph->nodes[i] = NULL;
    }

    return graph;
}

Node** getnodes(FILE* file, int numnodes, int dim) {
    char* line = NULL;
    size_t len = 0;
    Node** nodes = (Node**)malloc(numnodes * sizeof(Node*));
    if (nodes == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numnodes; i++) {
        if (getline(&line, &len, file) == -1) {
            fprintf(stderr, "Error reading line from file\n");
            exit(EXIT_FAILURE);
        }

        double* coords = (double*) malloc (dim * sizeof(double));
        if (coords == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        char* token = strtok(line, " ");
        for (int j = 0; j < dim && token != NULL; j++) {
            sscanf(token, "%le", &coords[j]);
            token = strtok(NULL, " ");
        }

        if (token != NULL) {
            fprintf(stderr, "Too many coordinates in line\n");
            exit(EXIT_FAILURE);
        }

        point* p = (point*)malloc(sizeof(point));
        if (p == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        setcoords(p, coords, dim);
        nodes[i] = create_node(i, p);

        free(coords);
    }

    free(line);  
    return nodes;
}


void addEdge(Graph* graph, Node* src, Node* dest) {
    
    ListNode* destNode = (ListNode*)malloc(sizeof(ListNode));           // Create a new list node for destination node
    destNode->node = dest;
    destNode->nextnode = NULL;

    
    if (src->kneighbors == NULL) {                                      // Add destination node to the kneighbors list of source node
        src->kneighbors = destNode;
    } else {
        ListNode* current = src->kneighbors;
        while (current->nextnode != NULL) {
            current = current->nextnode;
        }
        current->nextnode = destNode;
    }


    ListNode* srcNode = (ListNode*)malloc(sizeof(ListNode));            // Create a new list node for source node (reverse neighbor)
    srcNode->node = src;
    srcNode->nextnode = NULL;


    if (dest->rneighbors == NULL) {                                     // Add source node to the rneighbors list of destination node
        dest->rneighbors = srcNode;
    } else {
        ListNode* current = dest->rneighbors;
        while (current->nextnode != NULL) {
            current = current->nextnode;
        }
        current->nextnode = srcNode;
    }


    int srcIndex = src->numnode;                                        // Update adjacency list for source node in the graph
    if (graph->nodes[srcIndex] == NULL) {
        graph->nodes[srcIndex] = src;
    }


    int destIndex = dest->numnode;                                      // Update adjacency list for destination node in the graph
    if (graph->nodes[destIndex] == NULL) {
        graph->nodes[destIndex] = dest;
    }
}


void printNeighbors(Graph* graph) {
    for (int i = 0; i < graph->numnodes; ++i) {
        Node* currentNode = graph->nodes[i];
        printf("Node %d coords: %f , %f \n",currentNode->numnode,currentNode->data->coord[0],currentNode->data->coord[1]);
        printf("Node %d kneighbors: ", currentNode->numnode);
        ListNode* kneighborNode = currentNode->kneighbors;
        while (kneighborNode != NULL) {
            printf("%d ", kneighborNode->node->numnode);
            kneighborNode = kneighborNode->nextnode;
        }
        printf("\n");

        printf("Node %d rneighbors: ", currentNode->numnode);
        ListNode* rneighborNode = currentNode->rneighbors;
        while (rneighborNode != NULL) {
            printf("%d ", rneighborNode->node->numnode);
            rneighborNode = rneighborNode->nextnode;
        }
        printf("\n");
    }
}

