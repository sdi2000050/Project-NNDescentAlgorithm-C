#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"


void nndescentlj(Graph* graph, int k, float (*distance_value)(point, point)) {
    int update = 1;
    int numnodes = graph->numnodes;
    //while(update) {                                                              // While the graph keeps getting updated do:
        update = 0;
        for(int i=0; i < numnodes; i++) {                                          // For each node in the graph
            ListNode* neighbors = connectlist(graph->nodes[i]->kneighbors,graph->nodes[i]->rneighbors);
            int numofneighbors = list_size(neighbors);
            initialize_arrays(graph->nodes[i], numofneighbors);

            for(int m = 0; m < numofneighbors; m++) {
                Node* currentnode = neighbors->node;
                ListNode* currentneighbors = neighbors->nextnode;
                graph->nodes[i]->ljarray[m].numnode = currentnode->numnode;
                
                for(int j = m; j < (numofneighbors-1); j++) { 
                    float dis = distance_value(*(currentnode->data), *(currentneighbors->node->data));
                    graph->nodes[i]->ljarray[m].neighbor_dist[j].dis = dis;
                    graph->nodes[i]->ljarray[j+1].neighbor_dist[m].dis = dis;
                    graph->nodes[i]->ljarray[j+1].neighbor_dist[m].node = currentnode->numnode;
                    graph->nodes[i]->ljarray[m].neighbor_dist[j].node = currentneighbors->node->numnode;
                    currentneighbors = currentneighbors->nextnode;
                }
                neighbors = neighbors->nextnode;
            }
        }

        for(int i=0; i < numnodes; i++){

        }
    //}
    for(int i=0; i < numnodes; i++) {                                          // For each node in the graph
            ListNode* neighbors = connectlist(graph->nodes[i]->kneighbors,graph->nodes[i]->rneighbors);
            int numofneighbors = list_size(neighbors);
            printf("node: %d\n",i);
            for(int m = 0; m < numofneighbors; m++) {
                printf("ljnodes %d: \n",graph->nodes[i]->ljarray[m].numnode);
                for(int j=0; j<(numofneighbors-1); j++){
                    printf(" %d dis -> %f ,",graph->nodes[i]->ljarray[m].neighbor_dist[j].node,graph->nodes[i]->ljarray[m].neighbor_dist[j].dis);
                }
                printf("\n");
            }
        }
}


void deletenode(ListNode** list, int node) {
    ListNode* curr = *list;
    ListNode* prev = NULL;

    if (curr != NULL && curr->node->numnode == node) {
        *list = curr->nextnode; 
        return;
    }

    while (curr != NULL && curr->node->numnode != node) {
        prev = curr;
        curr = curr->nextnode;
    }

    if (curr == NULL) {
        return;
    }

    prev->nextnode = curr->nextnode;
    free(curr);
}



void updateneighbors(Graph* graph, int node) {
    for (int i = 0; i < graph->numnodes; i++) {
        deletenode(&(graph->nodes[i]->rneighbors), node);
    }
}

int compareKDistance(const void* a, const void* b) {
    float disA = (*(KDistance**)a)->dis;
    float disB = (*(KDistance**)b)->dis;

    if (disA < disB) return -1;
    if (disA > disB) return 1;
    return 0;
}

void sort(KDistance** kd, int k) {
    qsort(kd, k, sizeof(KDistance*), compareKDistance);
}

int notinarray(int numnode, KDistance** kd, int k){
    for(int i=0; i<k; i++){
        if(kd[i]->node == numnode){
            return 0;
        }
    }
    return 1;
}
/*
void checkneighbors(Node* node, ListNode* neighbors, KDistance** kd, int k, float (*distance_value)(point, point)){
    ListNode* curr = neighbors;
    while(curr!=NULL){
        if (node->numnode != curr->node->numnode){                      // If the current node isn't the node we compute distances for
            float dis = distance_value(*(curr->node->data),*(node->data)); 
            if( kd[k-1]->dis > dis && notinarray(curr->node->numnode,kd,k) == 1){
                kd[k-1]->node = curr->node;                             // Check if the last node in array (aka the farthest neighbor so far) have greater distance than the new distance
                kd[k-1]->dis = dis;
                sort(kd, k);
            }
        }
        curr=curr->nextnode;
    }
    free(curr);
}

void checkneighborspoint(point p, ListNode* neighbors, KDistance** kd, int k, float (*distance_value)(point, point)){
    ListNode* curr = neighbors;
    while(curr!=NULL){
        float dis = distance_value(*(curr->node->data),p);
        if( kd[k-1]->dis > dis && notinarray(curr->node->numnode,kd,k) == 1){
            kd[k-1]->node = curr->node;
            kd[k-1]->dis = dis;
            sort(kd, k);
        }
        curr=curr->nextnode;
    }
    free(curr);
}*/

int exist(int numnode, ListNode* list){
    ListNode* current = list;
    while(current!=NULL){                                           // Check if node with numnode is already in list
        if (current->node->numnode == numnode){
            return 1;
        }
        current = current->nextnode;
    }
    return 0;
}

ListNode* connectlist(ListNode* a, ListNode* b){
    ListNode* c = NULL;
    ListNode* currc = NULL;

    while ( a != NULL){                                             // Connect lists a and b
        if(exist(a->node->numnode,c) == 0){
            ListNode* newnode = (ListNode*) malloc (sizeof(ListNode));
            newnode->node = create_node(a->node->numnode,a->node->kneighbors,a->node->rneighbors,a->node->data);
            newnode->nextnode = NULL;    

            if( c == NULL){
                c = newnode;
                currc = newnode;
            }else{
                currc->nextnode = newnode;
                currc = newnode;
            }

        }    
        a = a->nextnode;
    }

    while ( b != NULL){
        if(exist(b->node->numnode,c) == 0){
            ListNode* newnode = (ListNode*) malloc (sizeof(ListNode));
            newnode->node = create_node(b->node->numnode,b->node->kneighbors,b->node->rneighbors,b->node->data);
            newnode->nextnode = NULL;    

            if( c == NULL){
                c = newnode;
                currc = newnode;
            }else{
                currc->nextnode = newnode;
                currc = newnode;
            }
        }    
        b = b->nextnode;
    }

    return c;
}



int main(void) {
    // Create a graph with 6 nodes
    Graph* graph = createGraph(6);

    // Creating sample nodes
    Node* nodes[6];
    for (int i = 0; i < 6; ++i) {
        point* p = (point*)malloc(sizeof(point));
        p->dim=2;
        float coords[2] = {i, i};  // Assigning some arbitrary coordinates for testing
        setcoords(p, coords, 2);
        nodes[i] = create_node(i,NULL,NULL,p);
    }

    // Connecting nodes randomly to form an initial graph
    createRandomGraph(graph, nodes, 3,euclidean_distance);

    // Print the initial graph
    printf("Initial Graph:\n");
    printNeighbors(graph);

    // Run nndescent algorithm
    nndescentlj(graph, 3,euclidean_distance);

    // Print the updated graph
    printf("\nGraph after NN Descent:\n");
    printNeighbors(graph);


    // Clean up: Free memory
    for (int i = 0; i < 6; ++i) {
        free(nodes[i]->data->coord);
        free(nodes[i]->data);
        free(nodes[i]);
    }
    free(graph->nodes);
    free(graph);

    return 0;
}