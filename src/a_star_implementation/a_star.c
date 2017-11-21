#include "constants.h"
#include "a_star.h"
#include "graph.h"
#include "a_star_implementation.h"
#include "graph_implementation.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

A_STAR * new_a_star() {
  A_STAR *as = malloc(sizeof(A_STAR));
  as->start = -1;
  as->target = -1;
  as->n_of_nodes = 0;
  as->distance_from_start = NULL;
  as->came_from = NULL;
  as->heap = new_heap();
  return as;
}

void set_start(A_STAR *a, int start) {
  a->start = start;
}

void set_target(A_STAR *a, int target) {
  a->target = target;
}

void prepare_a_star(A_STAR *a, GRAPH *g) {
  int g_num = g->number_of_nodes,i;
  if (a->n_of_nodes == 0) {
	a->distance_from_start = malloc(g_num * sizeof(double));
	a->came_from = malloc(g_num * sizeof(int));
  } else if (g_num != a->n_of_nodes) {
	a->distance_from_start = realloc(a->distance_from_start, g_num * sizeof(double));
	a->came_from = realloc(a->came_from, g_num * sizeof(int));
  }
  a->n_of_nodes = g_num;
  for (i=0; i<g_num; i++) {
	a->distance_from_start[i] = -1.0;
	a->came_from[i] = -1;
  }
}

/* assuming that the box is prepared etc */
int search_path(A_STAR *a, GRAPH *g) {
  EDGE *e;
  int index,start = a->start, target = a->target;
  double dist;
  if (a->start < 0 || a->target < 0) {
	return FAIL;
  }
  prepare_a_star(a, g);
  a->distance_from_start[start] = 0.0;
  a->came_from[start] = start;
  index = start;

  do {
	int i;
	e = get_edges(g, get_vertex(g, index));
	for (i=0; i < g->n_of_edges[index]; i++) {
	  int edge_index = e[i].node->index;
	  double distance = a->distance_from_start[index] + heuristic(&(g->nodes[index]), &(g->nodes[edge_index]), g);

	  /* if found first time (distance < 0) and also if we found a shorter path then we add it */
	  if (a->came_from[edge_index] < 0 || distance < a->distance_from_start[edge_index]) { 
		a->distance_from_start[edge_index] = distance; /* shortest distance */
		a->came_from[edge_index] = index; /* shortest came from */
		printf("inserting %d\n", edge_index);
		/* 
		   There will be some with the same index but different distances,
		   but probably this is still more efficient compared to the other option
		   where the index is always updated in some list; at least this is easier to implement.
		   More memory used in the heap (total memory usage might be still smaller) but less swapping of indexes. 
		*/
		insert_component(a->heap, edge_index, distance + heuristic(&(g->nodes[edge_index]), &(g->nodes[a->target]), g));
	  }
	}

	do {
	  dist = get_first_value(a->heap);
	  index = get_first_index(a->heap);
	  printf("removing %d\n", index);
	  remove_first(a->heap);
	} while (dist > a->distance_from_start[index]); /* lets skip all the longer (old) paths */
	
  } while (a->heap->n_of_components > 0 && index != target);

  if (a->came_from[target] >= 0) {
	return SUCCESS;
  }
  return FAIL;
}

double get_path_length(A_STAR *a) {
  return a->distance_from_start[a->target];
}

int * get_path_indexes(A_STAR *a, int *steps) {
  int index = a->target;
  int size = 10;
  int *indexes = malloc(size * sizeof(int));
  int i;

  if (a->came_from[index] < 0) {
	*steps = -1;
	free(indexes);
	return NULL;
  }

  for (i=0; i < a->n_of_nodes; i++) {
	if (i >= size) {
	  size *= 2;
	  indexes = realloc(indexes, size * sizeof(int));
	}
	indexes[i] = index;
	*steps = i+1;
	if (index == a->start) {
	  break;
	}
	index = a->came_from[index];
  }
  
  return indexes;
}

void delete_a_star(A_STAR *a) {
  delete_heap(a->heap);
  free(a->came_from);
  free(a->distance_from_start);
}
