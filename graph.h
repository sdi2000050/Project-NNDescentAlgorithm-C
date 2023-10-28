#include "graphtypes.h"

point* create_point(int d);
void setcoords(point *p, float* coords, int dimention);
void printpoint(point point);

Node* create_node(int n, point* p);
Node** getnodes(char* filename, int* numnodes, int dim);
Graph* createGraph(int numnodes);
void createRandomGraph(Graph* graph, Node** nodes, int k);
int addEdge(Graph* graph, Node* src, Node* dest);
void printNeighbors(Graph* graph);
int exist(int numnode, ListNode* list);
ListNode* connectlist(ListNode* a, ListNode* b);