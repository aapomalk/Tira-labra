#include "unity.h"
#include "../src/constants.h"
#include "../src/graph.h"
#include "../src/graph_implementation.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define TEST_GRAPH_INDEX(g) do { \
	int i; \
	for (i=0; i<(g)->number_of_nodes; i++) { \
		TEST_ASSERT_EQUAL_INT(i, (g)->nodes[i].index); \
	} \
} while (0) /* note that g needs to be a GRAPH */

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
	TEST_ASSERT_EQUAL_INT(SUCCESS, delete_hydrogens(v));
	TEST_ASSERT_EQUAL_INT(FAIL, delete_hydrogens(v));
	free(v); /* if a vertex is not inserted in graph it needs to be freed */
}

void test_heuristic(void) {
	VERTEX *a = new_vertex();
	VERTEX *b = new_vertex();
	a->not_hydrogen.coord[0] = 1.0;
	b->not_hydrogen.coord[0] = 2.0;
	TEST_ASSERT_EQUAL_FLOAT(1.0, heuristic(a, b));
	a->not_hydrogen.coord[1] = 1.0;
	TEST_ASSERT_EQUAL_FLOAT(sqrt(2.0), heuristic(a, b));
	free(a);
	free(b);
}

void test_graph(void) {
	GRAPH *g = new_graph();
	TEST_ASSERT_EQUAL_INT(0, size_of_graph(g));
	TEST_ASSERT_EQUAL_INT(INITIAL_LENGTH, g->list_length);
	TEST_ASSERT(NULL != g->box);
	TEST_ASSERT(NULL != g->nodes);
	TEST_ASSERT(NULL != g->edges);
	TEST_ASSERT_EQUAL_INT(0, number_of_domains(g->box));
	TEST_ASSERT_EQUAL_INT(SUCCESS, delete_graph(&g));
	TEST_ASSERT(NULL == g);
}

void test_vertex_graph(void) {
	VERTEX *a = new_vertex();
	VERTEX *b = new_vertex();
	VERTEX *c = new_vertex();
	GRAPH *g = new_graph();
	a->not_hydrogen.index = 4;
	a->not_hydrogen.coord[0] = 3.0;
	initialize_hydrogens(a, 1);
	a->hydrogens[0].coord[0] = 8.0;
	TEST_ASSERT_EQUAL_FLOAT(8.0, a->hydrogens[0].coord[0]);
	b->not_hydrogen.index = 3;
	b->not_hydrogen.coord[0] = 5.0;
	c->not_hydrogen.index = 1;
	TEST_ASSERT_EQUAL_FLOAT(3.0, a->not_hydrogen.coord[0]);
	TEST_ASSERT_EQUAL_INT(-1, a->index); /* original index outside of g */
	TEST_ASSERT_EQUAL_INT(SUCCESS, add_vertex(g, a));
	TEST_ASSERT_EQUAL_FLOAT(8.0, g->nodes[0].hydrogens[0].coord[0]); /* a (note that a doesn't exist outside of g anymore) */
	TEST_GRAPH_INDEX(g); /* tests that the indexes are correct */
	TEST_ASSERT_EQUAL_FLOAT(3.0, g->nodes[0].not_hydrogen.coord[0]); /* a */
	TEST_ASSERT_EQUAL_INT(SUCCESS, add_vertex(g, b));
	TEST_GRAPH_INDEX(g);
	TEST_ASSERT_EQUAL_INT(1, g->nodes[1].index); /* a, notice the changed index */
	TEST_ASSERT_EQUAL_FLOAT(8.0, g->nodes[1].hydrogens[0].coord[0]); /* a */
	TEST_ASSERT_EQUAL_FLOAT(3.0, g->nodes[1].not_hydrogen.coord[0]); /* a */
	TEST_ASSERT_EQUAL_FLOAT(5.0, g->nodes[0].not_hydrogen.coord[0]); /* b */
	add_vertex(g, c);
	TEST_GRAPH_INDEX(g);
	TEST_ASSERT_EQUAL_FLOAT(3.0, g->nodes[2].not_hydrogen.coord[0]); /* a */
	delete_graph(&g);
}

void test_insert_many_vertex_to_graph(void) {
	int i,j=INITIAL_LENGTH;
	GRAPH *g = new_graph();
	for (i=0; i<100; i++) {
		VERTEX *v = new_vertex();
		TEST_ASSERT_EQUAL_INT(i, size_of_graph(g));
		TEST_ASSERT_EQUAL_INT(SUCCESS, add_vertex(g, v));
		if (i >= j - 1) {
			j = j * 2;
		}
		TEST_ASSERT_EQUAL_INT(j, g->list_length);
	}
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_vertex);
	RUN_TEST(test_heuristic);
	RUN_TEST(test_graph);
	RUN_TEST(test_vertex_graph);
	RUN_TEST(test_insert_many_vertex_to_graph);
	return UNITY_END();
}