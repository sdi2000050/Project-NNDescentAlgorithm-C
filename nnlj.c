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

            for(int k = 0; k < numofneighbors; k++) {
                Node* currentnode = neighbors->node;
                ListNode* currentneighbors = neighbors->nextnode;
                graph->nodes[i]->ljarray[k].numnode = currentnode->numnode;
                
                for(int j = 0; j < list_size(currentneighbors); j++) {
                    float dis = distance_value(*(currentnode->data), *(currentneighbors->node->data));
                    graph->nodes[i]->ljarray[k].neighbor_dist[j].dis = dis;
                    graph->nodes[i]->ljarray[k].neighbor_dist[j].node = currentneighbors->node;
                    currentneighbors = currentneighbors->nextnode;
                }
                neighbors = neighbors->nextnode;
            }
        }
}

