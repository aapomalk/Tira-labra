#include "unity.h" /* I have downloaded unity from http://www.throwtheswitch.org/unity/ */
#include "a_star.h"
#include "heap.h"
#include "graph.h"
#include "graph_implementation.h"
#include "constants.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void test_new_a_star(void) {
  A_STAR *a = new_a_star();
  TEST_ASSERT_EQUAL_INT(-1, a->start);
  TEST_ASSERT_EQUAL_INT(-1, a->target);
  TEST_ASSERT_EQUAL_INT(0, a->n_of_nodes);
  TEST_ASSERT(NULL == a->distance_from_start);
  TEST_ASSERT(NULL == a->came_from);
  delete_a_star(a);
}

void test_set_start_target(void) {
  A_STAR *a = new_a_star();
  set_start(a, 3);
  set_target(a, 5);
  TEST_ASSERT_EQUAL_INT(3, a->start);
  TEST_ASSERT_EQUAL_INT(5, a->target);
  delete_a_star(a);
}

void test_path_finding(void) {
  GRAPH *g = new_graph();
  A_STAR *a = new_a_star();
  int i,j,k,decomposition[DIMENSIONS] = {10, 10, 10};
  COORDINATE vectors[DIMENSIONS];
  
  for (i=0; i<DIMENSIONS; i++) {
	for (j=0; j<DIMENSIONS; j++) {
	  vectors[j][i] = 0.0;
	}
  }
  vectors[0][0] = 10.0;
  vectors[1][1] = 10.0; /* our box is a cube */
  vectors[2][2] = 10.0;
  for (i=0; i<decomposition[0]; i++) {
	for (j=0; j<decomposition[1]; j++) {
	  for (k=0; k<decomposition[2]; k++) {
		int a,b;
		VERTEX *v = new_vertex();
		v->not_hydrogen.coord[0] = 0.5 + i;
		v->not_hydrogen.coord[1] = 0.5 + j;
		v->not_hydrogen.coord[2] = 0.5 + k;
		initialize_hydrogens(v, 6);
		for (a=0; a<6; a++) {
		  for (b=0; b<DIMENSIONS; b++) {
			v->hydrogens[a].coord[b] = v->not_hydrogen.coord[b];
		  }
		}
		v->hydrogens[0].coord[0] += 0.5;
		v->hydrogens[1].coord[0] -= 0.5;
		v->hydrogens[2].coord[1] += 0.5;
		v->hydrogens[3].coord[1] -= 0.5;
		v->hydrogens[4].coord[2] += 0.5;
		v->hydrogens[5].coord[2] -= 0.5;
		add_vertex(g, v);
	  }
	}
  }
  prepare_box(g, decomposition, vectors);
  set_distance(g, 0.5);
  set_angle(g, 20.0);

  set_start(a, 0);
  set_target(a, 500);

  /*TEST_ASSERT_EQUAL_INT(FAIL, search_path(a, g));*/ /* path is not found */
  set_distance(g, 1.5);
  i=search_path(a, g);
  /*TEST_ASSERT_EQUAL_INT(SUCCESS, i);*/ /* path is found */
  for (i=0; i < g->number_of_nodes; i++) {
	double dist = a->distance_from_start[i];
	int prev = a->came_from[i];
	if (prev >= 0) {
	  printf("distance: %f, prev: %d, index: %d\n", dist, prev, i);
	}
  }
}

int main(void) {
	printf("\n");
	UNITY_BEGIN();
	RUN_TEST(test_new_a_star);
	RUN_TEST(test_set_start_target);
	RUN_TEST(test_path_finding);
	return UNITY_END();
}
