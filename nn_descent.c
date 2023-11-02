#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"


void nndescent(Graph* graph, int k){
    int update = 1;
    int numnodes = graph->numnodes;
    while(update){
        update = 0;
        for(int i=0; i<numnodes; i++){
            Node* currentnode = graph->nodes[i];
            ListNode* neighbors = connectlist(currentnode->kneighbors,currentnode->rneighbors);
            ListNode* currentneighbor = neighbors;
            KDistance** kd = (KDistance**) malloc (k * sizeof(KDistance*));
            ListNode* curr = currentnode->kneighbors;
            for(int m=0; m<k; m++){
                kd[m]=(KDistance*)malloc(sizeof(KDistance));
                kd[m]->node = curr->node;
                kd[m]->dis = euclidean_distance(*(currentnode->data),*(curr->node->data));
                curr=curr->nextnode;
            }
            sort(kd,k);
            while(currentneighbor != NULL){
                ListNode* nneighbors = connectlist(currentneighbor->node->kneighbors,currentneighbor->node->rneighbors);
                checkneighbors(currentnode,nneighbors,kd,k);
                int count=0;
                for(int l=0; l<k; l++){
                    if (exist(kd[l]->node->numnode,currentnode->kneighbors) == 1){
                        count++;
                    }
                }
                if (count != k){
                    currentnode->kneighbors = NULL;
                    updateneighbors(graph,currentnode->numnode);
                    for(int j=0; j<k; j++){
                        addEdge(graph,currentnode,kd[j]->node);
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
        for (int j = 0; j < k - i - 1; ++j) {
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

void checkneighbors(Node* node, ListNode* neighbors, KDistance** kd, int k){
    ListNode* curr = neighbors;
    while(curr!=NULL){
        if (node->numnode != curr->node->numnode){
            float dis = euclidean_distance(*(curr->node->data),*(node->data));
            if( kd[k-1]->dis > dis && notinarray(curr->node->numnode,kd,k) == 1){
                kd[k-1]->node = curr->node;
                kd[k-1]->dis = dis;
                sort(kd, k);
            }
        }
        curr=curr->nextnode;
    }
}


int exist(int numnode, ListNode* list){
    ListNode* current = list;
    while(current!=NULL){
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

    while ( a != NULL){
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

