#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"


void local_join(Graph* graph, int k, float (distance_value)(point, point)) {
    int update = 1;
    int numofnodes = graph->numnodes;
    double d = 0.001;
    double p = 0.5;
    int updatecounts=numofnodes;

    while(update && updatecounts > (numofnodes*k*d) ) {
        updatecounts=0;
        update = 0;
        ListNode** new = (ListNode**) malloc(numofnodes * sizeof(ListNode*));
        ListNode** old = (ListNode**) malloc(numofnodes * sizeof(ListNode*));
        for(int i=0; i<numofnodes; i++){
            new[i] = (ListNode*) malloc (sizeof(ListNode));
            old[i] = (ListNode*) malloc (sizeof(ListNode));
        }

        for(int i = 0; i < numofnodes; i++) {
            Node* rednode = graph->nodes[i];

            ListNode* new_kneighbors = true_neighbors(rednode->kneighbors);         // Sample kneighbors
            ListNode* old_kneighbors = false_neighbors(rednode->kneighbors);
            int pk = p * k;
            ListNode* new_pk = getpk(pk,new_kneighbors);

            ListNode* new_rneighbors = true_neighbors(rednode->rneighbors);         // Sample rneighbors
            ListNode* old_rneighbors = false_neighbors(rednode->rneighbors);
            ListNode* new_pr = getpk(pk,new_rneighbors);

            new[i] = connectlist(new_pk,new_pr);
            old[i] = connectlist(old_kneighbors,old_rneighbors);
        }

        for(int i=0; i<numofnodes; i++){

            ListNode* neighbors = connectlist(new[i],old[i]);

            int num = list_size(new[i]);
            for(int j = 0; j < num; j++) {
                Node* currentnode = neighbors->node;
                ListNode* currentneighbors = neighbors->nextnode;

                while(currentneighbors != NULL) {
                    /*if(neighbors->flag == false && currentneighbors->flag == false) {
                        currentneighbors = currentneighbors->nextnode;
                        continue;
                    }*/
                    float dis = distance_value(*(currentnode->data),*(currentneighbors->node->data));

                    if(dis < currentnode->ljarray[k-1]->dis && notinarray(currentneighbors->node->numnode,currentnode->ljarray,k)) {
                        currentnode->ljarray[k-1]->node = currentneighbors->node;
                        currentnode->ljarray[k-1]->dis = dis;
                        sort(currentnode->ljarray, k);
                    }
                    if(dis < currentneighbors->node->ljarray[k-1]->dis && notinarray(currentnode->numnode,currentneighbors->node->ljarray,k)) {
                        currentneighbors->node->ljarray[k-1]->node = currentnode;
                        currentneighbors->node->ljarray[k-1]->dis = dis;
                        sort(currentneighbors->node->ljarray, k);
                    }

                    currentneighbors = currentneighbors->nextnode;
                }
                neighbors = neighbors->nextnode;
            }
        }

        for(int i = 0; i < numofnodes; i++) {
            int count=0;
            for(int l = 0; l < k; l++) {                                         // Check if the results of our search are different from the already k neighbors of our node
                if (exist(graph->nodes[i]->ljarray[l]->node->numnode,graph->nodes[i]->kneighbors) == 1) {   //Node is already in k neighbors so it participate in local join
                    count++;
                    graph->nodes[i]->ljarray[l]->flag = false;
                } 
                else {
                    graph->nodes[i]->ljarray[l]->flag = true;
                }
            }
            if (count != k) {                                                    // If they are update the graph
                graph->nodes[i]->kneighbors = NULL;                              // Delete the current k neighbors
                updateneighbors(graph,graph->nodes[i]->numnode);                 // Delete this node from it's current reverse neighbors
                for(int j = 0; j < k; j++){
                    addEdge(graph,graph->nodes[i],graph->nodes[i]->ljarray[j]->node, graph->nodes[i]->ljarray[j]->flag);                 // Make the new edges between the k nearest nodes in array
                }
                update = 1;
                updatecounts++;
            }
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
        if(kd[i]->node->numnode == numnode){
            return 0;
        }
    }
    return 1;
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
            newnode->node = create_node(a->node->numnode,a->node->kneighbors,a->node->rneighbors,a->node->ljarray,a->node->data);
            newnode->flag = a->flag;
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
            newnode->node = create_node(b->node->numnode,b->node->kneighbors,b->node->rneighbors,b->node->ljarray,b->node->data);
            newnode->flag = b->flag;
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


ListNode* true_neighbors(ListNode* list){
    ListNode* curr = list;
    ListNode* true_n = (ListNode*) malloc (sizeof(ListNode));
    while(curr != NULL){
        if(curr->flag == true){
            if( true_n == NULL){
                true_n = curr;
            } else{
                ListNode* currtrue = true_n;
                while(currtrue->nextnode != NULL){
                    currtrue = currtrue->nextnode;
                }
                currtrue->nextnode = curr;
            }
        }
        curr = curr->nextnode;
    }
    free(curr);
    return true_n;
}

ListNode* false_neighbors(ListNode* list){
    ListNode* curr = list;
    ListNode* false_n = (ListNode*) malloc (sizeof(ListNode));
    while(curr != NULL){
        if(curr->flag == false){
            if( false_n == NULL){
                false_n = curr;
            } else{
                ListNode* currfalse = false_n;
                while(currfalse->nextnode != NULL){
                    currfalse = currfalse->nextnode;
                }
                currfalse->nextnode = curr;
            }
        }
        curr = curr->nextnode;
    }
    free(curr);
    return false_n;
}

ListNode* getpk(int pk, ListNode* list){
    if(list_size(list) <= pk){
        return list;
    }else{
        ListNode* curr = list;
        for(int i=0; i<pk; i++){
            curr = curr ->nextnode;
        }
        curr->nextnode = NULL;
        return list;
    }
}