#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing
#include "graph.h"
#include "math.h"

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
	free(p->coord);
	free(p); 
}

void test_createNode() {
	float coor[] = {1.2, 2.6};
	point* p = (point*) malloc(sizeof(point));
	setcoords(p, coor, 2);

	Node* node = create_node(0, NULL, NULL, NULL, p);

	TEST_ASSERT(node != NULL);
	TEST_ASSERT(node->data->coord[0] == coor[0] && node->data->coord[1] == coor[1]);
    free(p->coord);
	free(p);
	free(node);
}

void test_createGraph() {

	Graph* graph = createGraph(0);

	TEST_ASSERT(graph != NULL);
	TEST_ASSERT(graph->numnodes == 0);
	free(graph->nodes);
    free(graph);
}

void test_graph_addedge(void) {
	int numofnodes = 4;
	Graph* graph = createGraph(numofnodes);
	float data[4][3] ={{0.0, 1.0, 0.0}, {1.0, 2.0, 3.0}, {2.0, 2.6, 2.9}, {3.0, 3.2, 3.3}}; 

	point* p0 = (point*) malloc(sizeof(point));
	setcoords(p0, data[0], 3);
	Node* n0 = create_node(0, NULL, NULL, NULL, p0);

	point* p1 = (point*) malloc(sizeof(point));
	setcoords(p1, data[1], 3);
	Node* n1 = create_node(1, NULL, NULL, NULL, p1);

	point* p2 = (point*) malloc(sizeof(point));
	setcoords(p2, data[2], 3);
	Node* n2 = create_node(2, NULL, NULL, NULL, p2);

	point* p3 = (point*) malloc(sizeof(point));
	setcoords(p3, data[3], 3);
	Node* n3 = create_node(3, NULL, NULL, NULL, p3);

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

void test_listsize() {
	int size = rand() % 100 + 1;
	ListNode* list = (ListNode*) malloc(sizeof(ListNode));
    ListNode* first = list;

	for(int i = 0; i < size; i++) {
		if(i == 0) {
			list->node = create_node(i, NULL, NULL, NULL, NULL);
		}
		else {
			list->nextnode = (ListNode*) malloc(sizeof(ListNode));
			list->nextnode->node = create_node(i, NULL, NULL, NULL, NULL);
			list = list->nextnode;
		}
	}

	if(list != NULL) {
		list->nextnode = NULL;
	}

    list = first; 
	TEST_ASSERT(list_size(list) == size);

	while(list != NULL) {
		ListNode* next = list->nextnode;
		free(list->node);
		free(list);
		list = next;
	}
}

void test_exist() {

	ListNode* a = (ListNode*) malloc(sizeof(ListNode));
	ListNode* b = (ListNode*) malloc(sizeof(ListNode));
	ListNode* c = (ListNode*) malloc(sizeof(ListNode));

	Node* n0 = create_node(0, NULL, NULL, NULL, NULL);
	Node* n1 = create_node(1, NULL, NULL, NULL, NULL);
	Node* n2 = create_node(2, NULL, NULL, NULL, NULL);

	a->node = n0;
	b->node = n1;
	c->node = n2;

	a->nextnode = b;
	b->nextnode = c;
	c->nextnode = NULL;

	for(int i = 0; i<3; i++) {
		TEST_ASSERT(exist(i, a) == 1);
	}
	for (int i = 0; i < 3; i++) {
		TEST_ASSERT(exist(10 - i, a) == 0);
	}

	while(a != NULL) {
		ListNode* next = a->nextnode;
		free(a->node);
		free(a);
		a = next;
	}
}


void test_connectlist() {
	ListNode* a = (ListNode*) malloc(sizeof(ListNode));
	ListNode* b = (ListNode*) malloc(sizeof(ListNode));
	ListNode* c = (ListNode*) malloc(sizeof(ListNode));

	Node* n0 = create_node(0, NULL, NULL, NULL, NULL);
	Node* n1 = create_node(1, NULL, NULL, NULL, NULL);
	Node* n2 = create_node(2, NULL, NULL, NULL, NULL);

	a->node = n0;
	b->node = n1;
	c->node = n2;

	a->nextnode = b;
	b->nextnode = c;
	c->nextnode = NULL;

	ListNode* d = (ListNode*) malloc(sizeof(ListNode));
	ListNode* e = (ListNode*) malloc(sizeof(ListNode));
	ListNode* f = (ListNode*) malloc(sizeof(ListNode));

	Node* n4 = create_node(0, NULL, NULL, NULL, NULL);
	Node* n5 = create_node(5, NULL, NULL, NULL, NULL);
	Node* n6 = create_node(6, NULL, NULL, NULL, NULL);

	d->node = n4;
	e->node = n5;
	f->node = n6;

	d->nextnode = e;
	e->nextnode = f;
	f->nextnode = NULL;

	ListNode* list;
	list = connectlist(a, d);

	TEST_ASSERT(list_size(list) == 5);

	while(list != NULL) {
		TEST_ASSERT(list->node != n5);
		list = list->nextnode;
	}
	while(list != NULL) {
		ListNode* next = list->nextnode;
		free(list->node);
		list = next;
	}
	free(list);
}

void test_deletenode() {
	int size = rand() % 100 + 1;
    ListNode** all = (ListNode**) malloc(sizeof(ListNode*));
	ListNode* list = (ListNode*) malloc(sizeof(ListNode));
    *all = list;
    ListNode* first = list;

	for(int i = 0; i < size; i++) {
		if(i == 0) {
			list->node = create_node(i, NULL, NULL, NULL, NULL);
		}
		else {
			list->nextnode = (ListNode*) malloc(sizeof(ListNode));
			list->nextnode->node = create_node(i, NULL, NULL, NULL, NULL);
			list = list->nextnode;
		}
	}

	if(list != NULL) {
		list->nextnode = NULL;
	}

    list = first;
	deletenode(all, 1);
    int i = rand() % size;
    if (i == 1) {
        i = i + 2;
    }
    deletenode(all, i);

    TEST_ASSERT(exist(i, *all) == 0);
	TEST_ASSERT(exist(1, *all) == 0);
	for(int j = 0; j < size; j++) {
		if(j != i && j != 1){
			TEST_ASSERT(exist(j, *all) == 1);
		}
	}

	while(list != NULL) {
		ListNode* next = list->nextnode;
		free(list->node);
		list = next;
	}
	free(all);
	free(list);
}

void test_notinarray() {
	int size = rand() % 50 +1;
	KDistance** kd = (KDistance**) malloc (size * sizeof(KDistance*));
	for(int i = 0; i < size; i++) {
		kd[i] = (KDistance*)malloc(sizeof(KDistance));
		kd[i]->node = create_node(i, NULL, NULL, NULL, NULL);
	}

	for(int i = 0; i < size; i++) {
		TEST_ASSERT(notinarray(i, kd, size) == 0 );
	}
	for(int i = size; i < 2*size; i++) {
		TEST_ASSERT(notinarray(i, kd, size) == 1 );
	}

	for(int i = 0; i < size; i++) {
		free(kd[i]->node);
		free(kd[i]);
	}
	free(kd);
}

#define epsilon 1e-6

void test_euclidean() {
	point* a = (point*) malloc(sizeof(point));
	float coora[] = {2.2, 3.5};
	setcoords(a, coora, 2);

	point* b = (point*) malloc(sizeof(point));
	float coorb[] = {1.4, 2.9};
	setcoords(b, coorb, 2);

	float dist = euclidean_distance(*a, *b);
	TEST_ASSERT(fabs(dist - 1.0) < epsilon);

	float coor1[] = {2.3, 1.5, 4.0};
	point* c = (point*) malloc(sizeof(point));
	setcoords(c, coor1, 3);

	float coor2[] = {3.7, 1.5, 5.4};
	point* d = (point*) malloc(sizeof(point));
	setcoords(d, coor2, 3);

	dist = euclidean_distance(*c, *d);
	TEST_ASSERT(fabs(dist - 1.9799) < epsilon);

	free(a->coord);
	free(b->coord);
	free(c->coord);
	free(d->coord);
	free(a);
	free(b);
	free(c);
	free(d);
}

void test_manhattan() {
	point* a = (point*) malloc(sizeof(point));
	float coora[] = {4.4, 2.7};
	setcoords(a, coora, 2);

	point* b = (point*) malloc(sizeof(point));
	float coorb[] = {3.2, 2.9};
	setcoords(b, coorb, 2);

	float dist = manhattan_distance(*a, *b);
	TEST_ASSERT(fabs(dist - 1.4) < epsilon);

	float coor1[] = {1.1, 2.8, 6.2};
	point* c = (point*) malloc(sizeof(point));
	setcoords(c, coor1, 3);

	float coor2[] = {2.5, 4.7, 7.1};
	point* d = (point*) malloc(sizeof(point));
	setcoords(d, coor2, 3);

	dist = manhattan_distance(*c, *d);
	TEST_ASSERT(fabs(dist - 4.2) < epsilon);

	free(a->coord);
	free(b->coord);
	free(c->coord);
	free(d->coord);
	free(a);
	free(b);
	free(c);
	free(d);
}

void test_chebyshev() {
	point* a = (point*) malloc(sizeof(point));
	float coora[] = {1.4, 3.9};
	setcoords(a, coora, 2);

	point* b = (point*) malloc(sizeof(point));
	float coorb[] = {3.2, 6.6};
	setcoords(b, coorb, 2);

	float dist = chebyshev_distance(*a, *b);
	TEST_ASSERT(fabs(dist - 2.7) < epsilon);

 	float coor1[] = {1.1, 2.8, 6.2};
	point* c = (point*) malloc(sizeof(point));
	setcoords(c, coor1, 3);

	float coor2[] = {2.5, 4.7, 7.1};
	point* d = (point*) malloc(sizeof(point));
	setcoords(d, coor2, 3);

	dist = chebyshev_distance(*c, *d);
	TEST_ASSERT(fabs(dist - 1.9) < epsilon);

	free(a->coord);
	free(b->coord);
	free(c->coord);
	free(d->coord);
	free(a);
	free(b);
	free(c);
	free(d); 
}

TEST_LIST = {
	{ "setcoords", test_pointcoords},
	{ "createNode", test_createNode},
	{ "createGraph", test_createGraph},	
	{ "addEdge", test_graph_addedge},
	{ "list_size", test_listsize},	
	{ "exist", test_exist},	
	{ "connectlist", test_connectlist},
	{ "deletenode", test_deletenode},	
	{ "notinarray", test_notinarray},
	{ "euclidean_distance", test_euclidean},
	{ "manhattan_distance", test_manhattan},
	{ "chebyshev_distance", test_chebyshev},	
	{ NULL, NULL } 
};