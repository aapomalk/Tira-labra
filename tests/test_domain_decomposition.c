#include "unity.h" /* I have downloaded unity from http://www.throwtheswitch.org/unity/ */
#include "constants.h"
#include "graph.h"
#include "graph_implementation.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


void test_domain_within_reach(void) {
	GRAPH *g = new_graph();
	int i,j,decomposition[DIMENSIONS] = {10, 10, 10};
	COORDINATE vectors[DIMENSIONS], coord;
	int *neighbour = malloc(DIMENSIONS * sizeof(int));
	int a,b,c;
	
	for (i=0; i<DIMENSIONS; i++) {
		for (j=0; j<DIMENSIONS; j++) {
			vectors[i][j] = 0.0;
		}
	}
	vectors[0][0] = 10.0;
	vectors[1][0] = -3.0; /* box is not a cube */
	vectors[1][1] = 10.0;
	vectors[2][2] = 10.0;
	set_distance(g, 1.0); /* this determines whether domain is within reach */
	prepare_box(g, decomposition, vectors);
	
	for (a=0; a<DIMENSIONS; a++) {
		coord[a] = 5.5;
	}
	coord[0] += (5.5/10) * vectors[1][0];
	
	for (a=0; a<pow(5, DIMENSIONS); a++) {
		int true = 1;
		char message[30];
		b=a;
		for (c=0; c<DIMENSIONS; c++) {
			int x = -2 + (b % 5);
			if (x < -1 || x > 1) { /* only the closest domains are within reach */
				true = 0;
			}
			neighbour[c] = x;
			b /= 5;
		}
		sprintf(message, "neighbour: %d %d %d", neighbour[0], neighbour[1], neighbour[2]);
		TEST_ASSERT_EQUAL_INT_MESSAGE(true, domain_is_within_reach(g, coord, neighbour), message);
	}
	
	free(neighbour);
	delete_graph(&g);
}

int main(void) {
  printf("\n");
  UNITY_BEGIN();
  RUN_TEST(test_domain_within_reach);
  return UNITY_END();
}
