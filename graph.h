#include "graphtypes.h"

//**********Points**********
void setcoords(point *p, float* coords, int dimention);
void printpoint(point point);

//**********Nodes**********
Node* create_node(int n,  ListNode* kn, ListNode* rn, KDistance** lj, point* p);
Node** getnodes(char* filename, int* numnodes, int dim);

//**********List**********
int list_size(ListNode* list);

//**********Graph**********
Graph* createGraph(int numnodes);
void createRandomGraph(Graph* graph, Node** nodes, int k);
int addEdge(Graph* graph, Node* src, Node* dest, bool flag);
void printNeighbors(Graph* graph);
void initialize_arrays(Graph* g, int k, float (distance_value)(point, point));

//**********Distances**********
float euclidean_distance(point x, point y);
float manhattan_distance(point x, point y);
float chebyshev_distance(point x, point y);

//**********NN Descent**********
void local_join(Graph* graph, int k, float (distance_value)(point, point));
//void nndescent(Graph* graph, int k, Distancefunc distance_value);
//KDistance** nndescentpoint(Graph* graph, int k, int node, point p, Distancefunc distance_value);
void deletenode(ListNode** list, int node);
void updateneighbors(Graph* graph, int node);
int notinarray(int numnode, KDistance** kd, int k);
void sort(KDistance** kd, int k);
void checkneighbors(Node* node, KDistance* neighbors, int size, KDistance** kd, int k, Distancefunc distance_value);
void checkneighborspoint(point p, ListNode* neighbors, KDistance** kd, int k, Distancefunc distance_value);
int exist(int numnode, ListNode* list);
ListNode* connectlist(ListNode* a, ListNode* b);
