#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "graph.h"


void setcoords(point *p, float* coords, int dimention) {
    p->dim = dimention;
    p->coord = (float*)malloc(dimention * sizeof(float));

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


Node** getnodes(char* filename, int* numnodes, int dim) {
    FILE *file = fopen(filename, "rb");  // Open the file in binary mode
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Read the number of points from the file
    int num_points;
    if (fread(&num_points, sizeof(int), 1, file) != 1) {
        fprintf(stderr, "Error reading the number of points from file\n");
        exit(EXIT_FAILURE);
    }

    point* points =(point*) malloc(num_points * sizeof(point));
    if (points == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_points; ++i) {
        float* values = malloc(dim * sizeof(float));
        if (values == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }

        // Read the floats for the current point from the file
        if (fread(values, sizeof(float), dim, file) != dim) {
            fprintf(stderr, "Error reading data from file\n");
            exit(EXIT_FAILURE);
        }

        setcoords(&points[i],values,dim);
    }

    Node** nodes = (Node**)malloc(num_points * sizeof(Node*));
    if (nodes == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_points; i++) {
        nodes[i] = create_node(i, &points[i]);
    }

    *numnodes = num_points;
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

