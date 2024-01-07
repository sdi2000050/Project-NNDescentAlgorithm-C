#include <stdbool.h>

typedef struct point{
    float *coord;
    int dim;
    float norm;
} point;

typedef float (*Distancefunc)(point, point);

typedef struct ListNode ListNode;
typedef struct kdistances KDistance;

typedef struct Node {
    int numnode;
    point* data;
    ListNode* kneighbors;
    ListNode* rneighbors;
    KDistance** ljarray; 
    bool flagrp;
} Node;

typedef struct ListNode {
    Node* node;
    ListNode* nextnode;
    bool flag;
} ListNode;

typedef struct Graph {
    int numnodes;
    Node** nodes;
} Graph;

typedef struct kdistances {
    Node* node;             
    float dis;
    bool flag;
} KDistance;

typedef struct jobscheduler JobS;
typedef struct job Job;


typedef struct randomprojectionargs {
    Graph* graph;
    Node** nodes;
    int dim;
    int k;
    int D;
    Distancefunc distance_function;
} RPargs;