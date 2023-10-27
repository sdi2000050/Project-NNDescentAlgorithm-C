#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing
#include "graph.h"


void test_pointcoords() {
	int N = 100;
	float array[N];
	for(int i = 0; i < N; i++) {
		array[i] = rand() % 100;
	} 

	point* p = (point*) malloc(sizeof(point));
	setcoords(p, array, N);

	TEST_ASSERT(p->coord[0] == array[0]);
	for(int i = 0; i < N; i++) {
		TEST_ASSERT(p->coord[i] == array[i]);
	} 
}

void test_createNode() {
	float coor[] = {1.0, 2.6};
	point* p = (point*) malloc(sizeof(point));
	setcoords(p, coor, 2);

	Node* node = create_node(0, p);

	TEST_ASSERT(node != NULL);
	TEST_ASSERT(node->data->coord[0] == 1.0 && node->data->coord[1] == 2.6);
    free(node);
}

void test_createGraph() {

	Graph* graph = createGraph(0);

	TEST_ASSERT(graph != NULL);
	TEST_ASSERT(graph->numnodes == 0);
    free(graph);
}

void test_graph_addedge(void) {
	int numofnodes = 4;
	Graph* graph = createGraph(numofnodes);
	float data[4][3] ={{0.0, 1.0, 0.0}, {1.0, 2.0, 3.0}, {2.0, 2.6, 2.9}, {3.0, 3.2, 3.3}}; 

	point* p0 = create_point(3);
	setcoords(p0, data[0], 3);
	Node* n0 = create_node(0, p0);

	point* p1 = create_point(3);
	setcoords(p1, data[1], 3);
	Node* n1 = create_node(1, p1);

	point* p2 = create_point(3);
	setcoords(p2, data[2], 3);
	Node* n2 = create_node(2, p2);

	point* p3 = create_point(3);
	setcoords(p3, data[3], 3);
	Node* n3 = create_node(3, p3);

	addEdge(graph, n0, n1);
	addEdge(graph, n1, n2);
	addEdge(graph, n1, n3);

	ListNode* l = n0->kneighbors;
	
	TEST_ASSERT(l->node->numnode == n1->numnode);
	TEST_ASSERT(l->nextnode == NULL);

	l = n1->kneighbors;
	TEST_ASSERT(l->node->numnode == n2->numnode);
	l = l->nextnode;
	TEST_ASSERT(l->node->numnode == n3->numnode);
	TEST_ASSERT(l->nextnode == NULL);

	l = n2->kneighbors;
	TEST_ASSERT(l == NULL);

	l = n3->kneighbors;
	TEST_ASSERT(l == NULL);	
}



TEST_LIST = {
	{ "setcoords", test_pointcoords},
	{ "createNode", test_createNode},
	{ "createGraph", test_createGraph},	
	{ "addEdge", test_graph_addedge},	
	{ NULL, NULL } 
};