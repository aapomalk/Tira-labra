#ifndef A_STAR_H
#define A_STAR_H
#include "graph.h"

typedef struct HEAP_COMPONENT {
  int index;
  double value;
} HEAP_COMPONENT;

typedef struct HEAP {
  HEAP_COMPONENT * components;
  int size, n_of_components;
} HEAP;

HEAP * new_heap();
void insert_component(HEAP *h, int index, double value);
int get_first_index(HEAP *h);
double get_first_value(HEAP *h);
void remove_first(HEAP *h);
void heap_new_value(HEAP *h, int heap_index, double value);
void delete_heap(HEAP *h);

typedef struct A_STAR {
  int start, target, n_of_nodes;
  double limiter; /* optional, to restrinct the distance from target */

  /* these are all for each node in GRAPH */
  double *distance_from_start;
  int *came_from;

  HEAP *heap;
} A_STAR;

A_STAR * new_a_star();
void set_limiter(A_STAR *a, double limiter);
void set_start(A_STAR *a, int start);
void set_target(A_STAR *a, int target);
int search_path(A_STAR *a, GRAPH *g); /* returns SUCCESS if found */
double get_path_length(A_STAR *a);
int * get_path_indexes(A_STAR *a, int *steps); /* steps = length of return list */
void delete_a_star(A_STAR *a);

#endif
