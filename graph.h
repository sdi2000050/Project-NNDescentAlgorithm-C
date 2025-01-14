#include "graphtypes.h"

//**********Points**********
void getpoints(void* args);
void setcoords(point *p, float* coords, int dimention);
float dot_product(point p1, point p2);
void printpoint(point point);

//**********Nodes**********
Node* create_node(int n,  ListNode* kn, ListNode* rn, KDistance** lj, point* p);
Node** getnodes(JobS* sch, char* filename, int* numnodes, int dim);

//**********List**********
int list_size(ListNode* list);

//**********Graph**********
Graph* createGraph(int numnodes);
void createRandomGraph(Graph* graph, Node** nodes, int k);
int addEdge(Graph* graph, Node* src, Node* dest, bool flag);
void printNeighbors(Graph* graph);
void initialize_arrays(Graph* g, int k, float (distance_value)(point, point));
int* splithyperplane(float vector[], int dim, int* subset, Node** nodes,int* size);
void randomprojection(void* args);
void getknodes(Graph* graph, int* subset,Node** nodes, int numnodes, int dim, int k, Distancefunc distance_function);
void create_pt_graph(Graph* graph, Node** nodes, int k);

//**********Distances**********
float euclidean_distance(point x, point y);
float manhattan_distance(point x, point y);
float chebyshev_distance(point x, point y);

//**********NN Descent**********
void local_join(Graph* graph, int k, float (distance_value)(point, point), double d, double p);
KDistance** nndescentpoint(Graph* graph, int k, int node, point p, Distancefunc distance_value);
void deletenode(ListNode** list, int node);
void updateneighbors(Graph* graph, int node);
int notinarray(int numnode, KDistance** kd, int k);
void sort(KDistance** kd, int k);
void checkneighborspoint(point p, ListNode* neighbors, KDistance** kd, int k, Distancefunc distance_value);
int exist(int numnode, ListNode* list);
ListNode* connectlist(ListNode* a, ListNode* b);
ListNode* true_neighbors(ListNode* list);
ListNode* false_neighbors(ListNode* list);
ListNode* getpk(int pk, ListNode* list);


//**********Job Scheduler*********
Job* jobcreate (void (*job)(void*), void* args);
JobS* initialize_scheduler(int execution_threads);
int submit_job(JobS* sch, Job* j);
void* execute(void* s);
int start_execute(JobS* sch);
int wait_all_tasks_finish(JobS* sch);
int destroy_scheduler(JobS* sch);