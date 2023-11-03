#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"


KDistance** nndescentpoint(Graph* graph, int k, int node, point p, float (*distance_value)(point, point)){
    int numnodes = graph->numnodes;
    KDistance** kd = (KDistance**) malloc (k * sizeof(KDistance*));
    ListNode* curr = graph->nodes[node]->kneighbors;                            // Initialize array with k nearest nodes starting from current node
    for(int m=0; m<k; m++){
        kd[m]=(KDistance*)malloc(sizeof(KDistance));
        kd[m]->node = curr->node;
        kd[m]->dis = distance_value(p,*(curr->node->data));
        curr=curr->nextnode;
    }
    sort(kd,k);                                                                 // Sort array of k nearest nodes
    for(int i=node; i<numnodes; i++){
        Node* currentnode = graph->nodes[i];                                    // Connect kneighbors and rneighbors list of current node
        ListNode* currentneighbor = connectlist(currentnode->kneighbors,currentnode->rneighbors);
        while(currentneighbor != NULL){
            ListNode* nneighbors = connectlist(currentneighbor->node->kneighbors,currentneighbor->node->rneighbors);
            checkneighborspoint(p,nneighbors,kd,k,distance_value);              // Check if the neighbor nodes are nearest to point 
            currentneighbor = currentneighbor->nextnode;
        }
    }
    return kd;
}


void nndescent(Graph* graph, int k, float (*distance_value)(point, point)){
    int update = 1;
    int numnodes = graph->numnodes;
    while(update){                                                              // While the graph keeps getting updated do:
        update = 0;
        for(int i=0; i<numnodes; i++){                                          // For each node in the graph
            Node* currentnode = graph->nodes[i];
            ListNode* currentneighbor = connectlist(currentnode->kneighbors,currentnode->rneighbors);
            KDistance** kd = (KDistance**) malloc (k * sizeof(KDistance*));     // Check the neighbors and reverse neighbors of it's one neighbors
            ListNode* curr = currentnode->kneighbors;
            for(int m=0; m<k; m++){
                kd[m]=(KDistance*)malloc(sizeof(KDistance));                    // Initialize the array to keep the k nearest nodes
                kd[m]->node = curr->node;
                kd[m]->dis = distance_value(*(currentnode->data),*(curr->node->data));
                curr=curr->nextnode;
            }
            sort(kd,k);
            while(currentneighbor != NULL){
                ListNode* nneighbors = connectlist(currentneighbor->node->kneighbors,currentneighbor->node->rneighbors);
                checkneighbors(currentnode,nneighbors,kd,k,distance_value);
                int count=0;
                for(int l=0; l<k; l++){                                         // Check if the results of our search are different from the already k neighbors of our node
                    if (exist(kd[l]->node->numnode,currentnode->kneighbors) == 1){
                        count++;
                    }
                }
                if (count != k){                                                // If they are update the graph 
                    currentnode->kneighbors = NULL;                             // Delete the current k neighbors
                    updateneighbors(graph,currentnode->numnode);                // Delete this node from it's current reverse neighbors 
                    for(int j=0; j<k; j++){
                        addEdge(graph,currentnode,kd[j]->node);                 // Make the new edges between the k nearest nodes in array
                    }
                    update = 1;
                    
                    
                }
                currentneighbor = currentneighbor->nextnode;                    
            }
        }
    }
}

void deletenode(ListNode** list, int node) {
    ListNode* curr = *list;
    ListNode* prev = NULL;

    if (curr != NULL && curr->node->numnode == node) {
        *list = curr->nextnode; 
        free(curr); 
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


void sort(KDistance** kd, int k) {
    for (int i = 0; i < k - 1; ++i) {
        for (int j = 0; j < k - i - 1; ++j) {                               // Asceding sorting based on distance
            if (kd[j]->dis > kd[j + 1]->dis) {
                KDistance* temp = kd[j];
                kd[j] = kd[j + 1];
                kd[j + 1] = temp;
            }
        }
    }
}

int notinarray(int numnode, KDistance** kd, int k){
    for(int i=0; i<k; i++){
        if(kd[i]->node->numnode == numnode){
            return 0;
        }
    }
    return 1;
}

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
}

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

