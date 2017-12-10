#include "constants.h"
#include "a_star.h"
#include "heap.h"
#include "allocation.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

HEAP * new_heap() {
  HEAP *h = allocation_malloc(1, sizeof(HEAP));
  h->size = INITIAL_HEAP_SIZE;
  h->n_of_components = 0;
  h->components = allocation_malloc(h->size, sizeof(HEAP_COMPONENT));
  return h;
}

int parent(int heap_index) {
  if (heap_index == 0) {
	return 0;
  }
  return (heap_index + 1) / 2 - 1;
}

int left_child(int heap_index) {
  return (heap_index + 1) * 2 - 1;
}

int right_child(int heap_index) {
  return (heap_index + 1) * 2;
}

void swap(HEAP *h, int id1, int id2) {
  HEAP_COMPONENT temp = h->components[id1];
  h->components[id1] = h->components[id2];
  h->components[id2] = temp;
}

void heapify(HEAP *h, int heap_index) {
  int parent_idx = parent(heap_index);
  double prnt = h->components[parent_idx].value;
  int left_idx, right_idx;
  while (heap_index > 0 && prnt >= h->components[heap_index].value) {
	swap(h, heap_index, parent_idx);

	heap_index = parent_idx;
	parent_idx = parent(heap_index);
	prnt = h->components[parent_idx].value;
  }

  left_idx = left_child(heap_index);
  right_idx = right_child(heap_index);
  while (left_idx < h->n_of_components || right_idx < h->n_of_components) {
	double left, right, current = h->components[heap_index].value;
	if (left_idx < h->n_of_components) {
	  left = h->components[left_idx].value;
	} else {
	  left = h->components[heap_index].value + 1.0; /* out of heap, by putting larger value prevents swap */
	}
	if (right_idx < h->n_of_components) {
	  right = h->components[right_idx].value;
	} else {
	  right = h->components[heap_index].value + 1.0; /* prevents swap */
	}
	if (right > current && left > current) {
	  break;
	}

	if (right < left) {
	  swap(h, heap_index, right_idx);
	  heap_index = right_idx;
	} else {
	  swap(h, heap_index, left_idx);
	  heap_index = left_idx;
	}

	left_idx = left_child(heap_index);
	right_idx = right_child(heap_index);
  }
}

void insert_component(HEAP *h, int index, double value) {
  int heap_index = h->n_of_components;
  if (h->n_of_components >= h->size) {
	h->size *= 2;
	h->components = allocation_realloc(h->n_of_components, h->components, h->size, sizeof(HEAP_COMPONENT));
  }
  h->components[heap_index].index = index;
  h->components[heap_index].value = value;
  h->n_of_components += 1;
  heapify(h, heap_index);
}

int get_first_index(HEAP *h) {
  if (h->n_of_components > 0) {
	return h->components[0].index;
  }
  return -1;
}

double get_first_value(HEAP *h) {
  if (h->n_of_components > 0) {
	return h->components[0].value;
  }
  return -1.0; /* assuming that the values are positive */
}

void remove_first(HEAP *h) {
  if (h->n_of_components > 0) {
	swap(h, 0, h->n_of_components - 1);
	h->n_of_components -= 1;
	heapify(h, 0);
  }
}

void heap_new_value(HEAP *h, int heap_index, double value) {
  if (h->n_of_components > heap_index) {
	h->components[heap_index].value = value;
	heapify(h, heap_index);
  }
}

void delete_heap(HEAP *h) {
  free(h->components);
  free(h);
}
