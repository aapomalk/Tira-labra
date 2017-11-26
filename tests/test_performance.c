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
  time_t t;
  int index = 0;
  int *indexes;
  srand((unsigned) time(&t));
  printf("%d\n", rand());
  
  for (i=0; i<DIMENSIONS; i++) {
	decomposition[i] = decomp;
	for (j=0; j<DIMENSIONS; j++) {
	  vectors[j][i] = 0.0;
	}
  }
  vectors[0][0] = 1.0 * size;
  vectors[1][1] = 1.0 * size; /* our box is a cube */
  vectors[2][2] = 1.0 * size;
  set_vertex_list_length(g, size * size * size);
  for (i=0; i<size; i++) {
	for (j=0; j<size; j++) {
	  for (k=0; k<size; k++) {
		int a,b;
		VERTEX *v = &(g->nodes[index]);
		if (v->index != index) {
		  printf("error, index not the same!!!\n");
		}
		v->not_hydrogen.coord[0] = 0.5 + i + rand() * 0.01 / RAND_MAX;
		v->not_hydrogen.coord[1] = 0.5 + j + rand() * 0.01 / RAND_MAX;
		v->not_hydrogen.coord[2] = 0.5 + k + rand() * 0.01 / RAND_MAX;
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
		index++;
		g->number_of_nodes = index;
	  }
	}
  }
  prepare_box(g, decomposition, vectors);
  set_distance(g, 1.5);
  set_angle(g, 20.0);

  j=9;
  set_start(a, j);
  i = size / 2 - 1 + size * size / 2 + size * size * size / 2;
  set_target(a, i);
  printf("\nstart: %d, target: %d\n", j, i);
  
  time_start = clock();
  i = search_path(a, g);
  time_end = clock();
  if (i == FAIL) {
	printf("path not found with size %d\n", size);
  }
  printf("path length: %f\n", get_path_length(a));
  indexes = get_path_indexes(a, &i);
  printf("path edges %d\n", i);
  for (j=i-1; j>=0; j--) {
	printf("%d, ", indexes[j]);
  }
  printf("\n");
  return time_end - time_start;
}


int main(int argc, char ** argv) {
  int size,decomp;
  clock_t evaluation;
  printf("%s\n", argv[0]);
  sscanf(argv[1], "%d", &size);
  sscanf(argv[2], "%d", &decomp);
  evaluation = test_performance(size, decomp);
  printf("evaluation with size %d and decomposition %d is %ld\n", size, decomp, evaluation);
  return 0;
}
