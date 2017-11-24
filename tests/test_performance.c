#include "unity.h" /* I have downloaded unity from http://www.throwtheswitch.org/unity/ */
#include "a_star.h"
#include "heap.h"
#include "graph.h"
#include "graph_implementation.h"
#include "constants.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

clock_t test_performance(int size, int decomp) {
  GRAPH *g = new_graph();
  A_STAR *a = new_a_star();
  int i,j,k,decomposition[DIMENSIONS];
  COORDINATE vectors[DIMENSIONS];
  clock_t time_start, time_end;
  
  for (i=0; i<DIMENSIONS; i++) {
	decomposition[i] = decomp;
	for (j=0; j<DIMENSIONS; j++) {
	  vectors[j][i] = 0.0;
	}
  }
  vectors[0][0] = 1.0 * size;
  vectors[1][1] = 1.0 * size; /* our box is a cube */
  vectors[2][2] = 1.0 * size;
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
  set_distance(g, 1.5);
  set_angle(g, 20.0);

  set_start(a, 0);
  set_target(a, size / 2 - 1 + size * size / 2 + size * size * size / 2);

  time_start = clock();
  i = search_path(a, g);
  time_end = clock();
  if (i == FAIL) {
	printf("path not found with size %d\n", size);
  }
  return time_end - time_start;
}


int main(int argc, char ** argv) {
  int size,decomp;
  clock_t evaluation;
  sscanf(argv[1], "%d", &size);
  sscanf(argv[2], "%d", &decomp);
  evaluation = test_performance(size, decomp);
  printf("evaluation with size %d and decomposition %d is %ld\n", size, decomp, evaluation);
  return 0;
}
