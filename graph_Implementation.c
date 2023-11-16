#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

int list_size(ListNode* list) {
    ListNode* first = list;
    int count = 0;
    while(list != NULL) {
        count++;
        list = list->nextnode;
    }
    list = first;
    return count;
}

Node* create_node(int n, ListNode* kn, ListNode* rn, point* p) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->numnode = n;                                              
    node->data = p;
    node->kneighbors = kn;
    node->rneighbors = rn;
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
    FILE *file = fopen(filename, "rb");                                 // Open the file in binary mode
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    
    int num_points;                                                     // Read the number of points from the file
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

        
        if (fread(values, sizeof(float), dim, file) != dim) {         // Read the floats coords for the current point from the file
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
        nodes[i] = create_node(i, NULL, NULL, &points[i]);
    }

    *numnodes = num_points;
    fclose(file);
    return nodes;
}

void createRandomGraph(Graph* graph, Node** nodes, int k) {
    srand(time(NULL));
    int numnodes = graph->numnodes;
    int exit;
    for (int i = 0; i < numnodes; ++i) {
        int count = 0;
        while (count < k) {
            int randomNeighbor = rand() % numnodes;                         // Random neighbor node index
            if (randomNeighbor != i) {
                exit = addEdge(graph, nodes[i], nodes[randomNeighbor]);
                if (exit == 0){                                             // Check if a new edge was created    
                    count++;
                }
            }
        }
    }
}

int addEdge(Graph* graph, Node* src, Node* dest) {
    
    ListNode* destNode = (ListNode*)malloc(sizeof(ListNode));               // Create a new list node for destination node
    destNode->node = dest;
    destNode->nextnode = NULL;

    if (src->kneighbors != NULL){                                           // Check if there is already an edge from source to destination
        ListNode* curr = src->kneighbors;
        if (curr->node->numnode == dest->numnode) {
            return 1;
        }
        while (curr->nextnode != NULL) {
            if (curr->node->numnode == dest->numnode) {
                return 1;
            }
            curr = curr->nextnode;
        }
        if (curr->node->numnode == dest->numnode) {
            return 1;
        }
    }
    
    if (src->kneighbors == NULL) {                                          // Add destination node to the kneighbors list of source node
        src->kneighbors = destNode;
    } else {
        ListNode* current = src->kneighbors;
        while (current->nextnode != NULL) {
            current = current->nextnode;
        }
        current->nextnode = destNode;
    }


    ListNode* srcNode = (ListNode*)malloc(sizeof(ListNode));                // Create a new list node for source node (reverse neighbor)
    srcNode->node = src;
    srcNode->nextnode = NULL;


    if (dest->rneighbors == NULL) {                                         // Add source node to the rneighbors list of destination node
        dest->rneighbors = srcNode;
    } else {
        ListNode* current = dest->rneighbors;
        while (current->nextnode != NULL) {
            current = current->nextnode;
        }
        current->nextnode = srcNode;
    }


    int srcIndex = src->numnode;                                            // Update adjacency list for source node in the graph
    if (graph->nodes[srcIndex] == NULL) {
        graph->nodes[srcIndex] = src;
    }


    int destIndex = dest->numnode;                                          // Update adjacency list for destination node in the graph
    if (graph->nodes[destIndex] == NULL) {
        graph->nodes[destIndex] = dest;
    }


    return 0;
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
        free(kneighborNode);
        free(rneighborNode);
    }
}

