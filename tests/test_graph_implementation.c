#include "unity.h"
#include "../src/constants.h"
#include "../src/graph.h"
#include "../src/graph_implementation.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void test_vertex(void) {
	int i;
	VERTEX *v;
	v = new_vertex();
	TEST_ASSERT_EQUAL_INT(-1, v->index);
	TEST_ASSERT(NULL == v->hydrogens);
	TEST_ASSERT_EQUAL_INT(-1, v->not_hydrogen.index);
	for (i=0; i<3; i++) {
		TEST_ASSERT_EQUAL_FLOAT(0.0, v->not_hydrogen.coord[0]);
	}
	i=3;
	TEST_ASSERT_EQUAL_INT(SUCCESS, initialize_hydrogens(v, i));
	TEST_ASSERT_EQUAL_INT(i, v->n_hydrogens);
	TEST_ASSERT_EQUAL_INT(-1, get_index(v));
	TEST_ASSERT_EQUAL_STRING("-1", print_vertex(v));
	TEST_ASSERT_EQUAL_INT(SUCCESS, delete_vertex(&v));
	TEST_ASSERT_EQUAL_INT(FAIL, delete_vertex(&v));
	TEST_ASSERT(NULL == v);
}

void test_heuristic(void) {
	VERTEX *a = new_vertex();
	VERTEX *b = new_vertex();
	a->not_hydrogen.coord[0] = 1.0;
	b->not_hydrogen.coord[0] = 2.0;
	TEST_ASSERT_EQUAL_FLOAT(1.0, heuristic(a, b));
	a->not_hydrogen.coord[1] = 1.0;
	TEST_ASSERT_EQUAL_FLOAT(sqrt(2.0), heuristic(a, b));
}

void test_graph(void) {
	GRAPH *g = new_graph();
	TEST_ASSERT_EQUAL_INT(0, size_of_graph(g));
	TEST_ASSERT(NULL == g->nodes);
	TEST_ASSERT(NULL == g->edges);
	TEST_ASSERT(NULL != g->box);
	TEST_ASSERT_EQUAL_INT(0, number_of_domains(g->box));
	TEST_ASSERT_EQUAL_INT(SUCCESS, delete_graph(&g));
	TEST_ASSERT(NULL == g);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_vertex);
	RUN_TEST(test_heuristic);
	RUN_TEST(test_graph);
	return UNITY_END();
}