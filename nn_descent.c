#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graph.h"

typedef struct kdistances{
    Node* node;
    float dis;
}KDistance;

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
            while(currentneighbor != NULL){
                checkneighbors(currentnode,currentneighbor->node->kneighbors,kd,k);
                checkneighbors(currentnode,currentneighbor->node->rneighbors,kd,k);
                count=0;
                for(int l=0; l<k; l++){
                    if (exist(kd[l]->node->numnode,currentnode->kneighbors) == 1){
                        count++;
                    }
                }
                if (count!=k){
                    currentnode->kneighbors = NULL;
                    for(int j=0; j<k; j++){
                        addEdge(graph,currentnode,kd[i]->node);
                    }
                    updaterneighbors(graph,currentnode->numnode);
                    update = 1;
                }
                currentneighbor = currentneighbor->nextnode;
            }
        }

    }
}

void updateneighbors(Graph* graph, int node){
    for (int i=0; i<graph->numnodes; )
}

float euclidean_distance(point x, point y) {
    float s = 0.0;
    for(int i = 0; i < x.dim; i++) {
        float a = x.coord[i] - y.coord[i];
        s = s + pow(a, 2);
    }
    return sqrt(s);
}

int sort(const void* a, const void* b) {
    float disA = ((KDistance*)a)->dis;
    float disB = ((KDistance*)b)->dis;
    if (disA < disB) return -1;
    if (disA > disB) return 1;
    return 0;
}

void checkneighbors(Node* node, ListNode* neighbors, KDistance** kd, int k){
    ListNode* curr = neighbors;
    count = 0;
    while(curr!=NULL){
        float dis = euclidean_distance(&(curr->node->data),&(node->data));
        if( count < k ){
            kd[count]->node = curr->node;
            qsort(kd, k, sizeof(KDistance), comparedis);
            count++;
        }
        else{
            if( kd[k-1]->dis > dis){
                kd[k-1]->node = curr->node;
                qsort(kd, k, sizeof(KDistance), comparedis);
            }
        }
        curr=curr->nextnode;
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

