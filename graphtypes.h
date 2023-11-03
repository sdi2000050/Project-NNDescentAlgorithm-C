typedef struct {
    float *coord;
    int dim;
} point;

typedef float (*Distancefunc)(point, point);

typedef struct ListNode ListNode;

typedef struct Node {
    int numnode;
    point* data;
    ListNode* kneighbors;
    ListNode* rneighbors;
} Node;

typedef struct ListNode {
    Node* node;
    ListNode* nextnode;
} ListNode;

typedef struct Graph {
    int numnodes;
    Node** nodes;
} Graph;

typedef struct kdistances{
    Node* node;
    float dis;
}KDistance;