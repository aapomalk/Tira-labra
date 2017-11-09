#include "unity.h"
#include "../src/constants.h"
#include "../src/graph.h"
#include "../src/graph_implementation.h"
#include <math.h>

void test_graph_implementation(void) {
	int i;
	VERTEX *v = new_vertex();
	TEST_ASSERT_EQUAL_INT(-1, v->index);
	TEST_ASSERT(NULL == v->hydrogens);
	TEST_ASSERT_EQUAL_INT(-1, v->not_hydrogen.index);
	for (i=0; i<3; i++) {
		TEST_ASSERT_EQUAL_FLOAT(0.0, v->not_hydrogen.coord[0]);
	}
	i=3;
	TEST_ASSERT_EQUAL_INT(SUCCESS, initialize_hydrogens(v, i));
	TEST_ASSERT_EQUAL_INT(i, v->n_hydrogens);
	TEST_ASSERT_EQUAL_INT(SUCCESS, delete_vertex(v));
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

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_graph_implementation);
	RUN_TEST(test_heuristic);
	return UNITY_END();
}