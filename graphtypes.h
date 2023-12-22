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