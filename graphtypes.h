#include <stdbool.h>

typedef struct {
    float *coord;
    int dim;
} point;

typedef float (*Distancefunc)(point, point);

typedef struct ListNode ListNode;
typedef struct kdistances KDistance;

typedef struct Node {
    int numnode;
    bool flag;
    point* data;
    ListNode* kneighbors;
    ListNode* rneighbors;
    KDistance** ljarray; 
} Node;

typedef struct ListNode {
    Node* node;
    ListNode* nextnode;
    //bool flag;
} ListNode;

typedef struct Graph {
    int numnodes;
    Node** nodes;
} Graph;

typedef struct kdistances{
    Node* node;             //Μπορει να γινει μονο Numnode!!!
    float dis;
} KDistance;