#include "graphtypes.h"

void setcoords(point *p, float* coords, int dimention);
void printpoint(point point);

Node* create_node(int n, point* p);
Node** getnodes(char* filename, int* numnodes, int dim);
Graph* createGraph(int numnodes);
void addEdge(Graph* graph, Node* src, Node* dest);
void printNeighbors(Graph* graph);