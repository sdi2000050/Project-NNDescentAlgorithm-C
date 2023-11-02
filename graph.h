#include "graphtypes.h"

//**********Points**********
//point* create_point(int d);
void setcoords(point *p, float* coords, int dimention);
void printpoint(point point);

//**********Nodes**********
Node* create_node(int n,  ListNode* kn, ListNode* rn, point* p);
Node** getnodes(char* filename, int* numnodes, int dim);

//**********Graph**********
Graph* createGraph(int numnodes);
void createRandomGraph(Graph* graph, Node** nodes, int k);
int addEdge(Graph* graph, Node* src, Node* dest);
void printNeighbors(Graph* graph);

//**********Distances**********
float euclidean_distance(point x, point y);
float manhattan_distance(point x, point y);

//**********NN Descent**********
void nndescent(Graph* graph, int k);
void deletenode(ListNode** list, int node);
void updateneighbors(Graph* graph, int node);
int notinarray(int numnode, KDistance** kd, int k);
void sort(KDistance** kd, int k);
void checkneighbors(Node* node, ListNode* neighbors, KDistance** kd, int k);
int exist(int numnode, ListNode* list);
ListNode* connectlist(ListNode* a, ListNode* b);