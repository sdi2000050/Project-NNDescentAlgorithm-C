#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "graph.h"


void setcoords(point *p, float* coords, int dimention) {
    p->dim = dimention;
    p->coord = (float*)malloc(dimention * sizeof(float));

    for(int i = 0; i < dimention; i++) {
        p->coord[i] = coords[i];
    }

    p->norm = dot_product(*p,*p);
}

float dot_product(point p1, point p2){
    float prod = 0.0;

    for(int i=0; i<p1.dim; i++){
        prod = prod + (p1.coord[i] * p2.coord[i]);
    }
    
    return prod;
}

void printpoint(point point) {
    for(int i = 0; i < point.dim ; i++) {
        printf("%f ", point.coord[i]);
    }
    printf("\nnorm: %f",point.norm);
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

Node* create_node(int n, ListNode* kn, ListNode* rn, KDistance** lj, point* p) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->numnode = n;                                              
    node->data = p;
    node->kneighbors = kn;
    node->rneighbors = rn;
    node->ljarray = lj;
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
        nodes[i] = create_node(i, NULL, NULL, NULL,&points[i]);
    }

    *numnodes = num_points;
    fclose(file);
    return nodes;
}

void normalvector(int p1, int p2, Node** nodes, float vector[]){
    point* point1 = nodes[p1]->data;
    point* point2 = nodes[p2]->data;
    
    for(int i=0; i<point1->dim; i++){
        vector[i] = point2->coord[i] - point1->coord[i];
    }

    double m = 0;
    for(int i=0; i<point1->dim; i++){
        m += vector[i] * vector[i];
    }

    m = sqrt(m);

    for(int i=0; i<point1->dim; i++){
        vector[i] /= m;
    }

}

int* splithyperplane(float vector[], int dim, int* subset, Node** nodes, int* size){
    double D = 0;
    for(int i=0; i<dim; i++){
        D += nodes[subset[0]]->data->coord[i] * vector[i];
    }

    int n = *size;
    int count = 0;
    int newsubset[n]; 
    for(int i=0; i<n; i++){
        double dotp = 0;
        for(int j=0; j<dim; j++){
            dotp += nodes[subset[i]]->data->coord[j] * vector[j];
        }
        if (dotp < D){
            newsubset[count] = subset[i];
            count++;
        }
    }

    *size = count;
    int* finalsubset = (int*) malloc (count * sizeof(int));
    for(int i=0; i<count; i++){
        finalsubset[i] = newsubset[i];
    }

    return finalsubset;
}

void randomprojection(Graph* graph, Node** nodes, int dim, int k, int D, Distancefunc distance_function) {
    srand(time(NULL));

    int size = graph->numnodes;
    int* subset = (int*) malloc (size * sizeof(int));
    for(int i=0; i < size; i++) {
        subset[i] = i;
    }
    int i,p1,j,p2;
    while( size > D ) {
        i = rand() % (size+1);
        p1 = subset[i];
        j = rand() % (size+1); 
        p2 = subset[j];
        while ( i==j ) {
            j = rand() % size;
            p2 = subset[j];
        }

        float vector[dim];
        normalvector(p1,p2,nodes,vector);
    
        splithyperplane(vector,dim,subset,nodes,&size);
    }
    printf("size:%d\n",size);
    for(int i=0; i<size; i++){
        printf("%d ",subset[i]);
    }
    printf("\n");

    getknodes(graph,subset,nodes,size,dim,k,distance_function);

    create_pt_graph(graph, nodes, k);

    free(subset);

}

void getknodes(Graph* graph, int* subset, Node** nodes, int numnodes, int dim, int k, Distancefunc distance_function) {

    for (int i = 0; i < numnodes; i++) {

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

                distances[j]->node = nodes[subset[j]];
                distances[j]->dis = distance_function(*(nodes[subset[i]]->data), *(nodes[subset[j]]->data));
            
        }

        sort(distances,numnodes);

        int exit;
        for (int l = 0; l < k; l++) {

            exit = addEdge(graph,nodes[subset[i]],distances[l+1]->node,true);

        }

        for (int j = 0; j < numnodes-1; j++) {
            free(distances[j]);
        }
        free(distances);

        nodes[subset[i]]->flagrp = true;
    }

    for (int i = 0; i < numnodes; ++i) {
        Node* currentNode = nodes[subset[i]];
        printf("Node %d kneighbors: ", subset[i]);
        ListNode* kneighborNode = currentNode->kneighbors;
        while (kneighborNode != NULL) {
            printf("%d ", kneighborNode->node->numnode);
            kneighborNode = kneighborNode->nextnode;
        }
        printf("\n");
        free(kneighborNode);
    }

}

void create_pt_graph(Graph* graph, Node** nodes, int k) {
    srand(time(NULL));
    int numnodes = graph->numnodes;
    int exit;
    for(int i = 0; i < numnodes; i++) {
        if(nodes[i]->flagrp == true) {
            continue;
        }
        else {
            int count = 0;
            while (count < k) {
                int randomNeighbor = rand() % numnodes;                         // Random neighbor node index
                if (randomNeighbor != i) {
                    exit = addEdge(graph,nodes[i], nodes[randomNeighbor], true);
                    if (exit == 0) {                                             // Check if a new edge was created    
                        count++;
                    }
                }
            }
        }
    }
}

// void createRandomGraph(Graph* graph, Node** nodes, int k) {
//     srand(time(NULL));
//     int numnodes = graph->numnodes;
//     int exit;
//     for (int i = 0; i < numnodes; ++i) {
//         int count = 0;
//         while (count < k) {
//             int randomNeighbor = rand() % numnodes;                         // Random neighbor node index
//             if (randomNeighbor != i) {
//                 exit = addEdge(graph,nodes[i], nodes[randomNeighbor], true);
//                 if (exit == 0) {                                             // Check if a new edge was created    
//                     count++;
//                 }
//             }
//         }
//     }
// }

int addEdge(Graph* graph, Node* src, Node* dest, bool flag) {
    
    ListNode* destNode = (ListNode*)malloc(sizeof(ListNode));               // Create a new list node for destination node
    destNode->node = dest;
    destNode->flag = flag;
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
    srcNode->flag = flag;
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

void initialize_arrays(Graph* g, int k,float (distance_value)(point, point)) {
    for(int i=0; i<g->numnodes; i++){
        g->nodes[i]->ljarray = (KDistance**) malloc (k * sizeof(KDistance*));     
        ListNode* curr = g->nodes[i]->kneighbors;
        for(int m=0; m < k; m++){
            g->nodes[i]->ljarray[m] = (KDistance*) malloc (sizeof(KDistance));
            g->nodes[i]->ljarray[m]->node = curr->node;
            g->nodes[i]->ljarray[m]->dis = distance_value(*(g->nodes[i]->data),*(curr->node->data));
            curr=curr->nextnode;
        }
        free(curr);
        sort(g->nodes[i]->ljarray,k);
    }
}
