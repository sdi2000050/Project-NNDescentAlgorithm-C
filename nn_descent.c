#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
            while(currentneighbor != NULL){


                currentneighbor = currentneighbor->nextnode;
            }
        }

    }
}


int exist(int numnode, ListNode* list){
    ListNode* current = (ListNode*) malloc(sizeof(ListNode));
    current = list;
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
            newnode->node = create_node(a->node->numnode,a->node->data);
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
            newnode->node = create_node(b->node->numnode,b->node->data);
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

