#include "graph.h"
#include "graph_implementation.h"
#include "constants.h"
#include "allocation.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

VERTEX * new_vertex() {
	int i;
	VERTEX *v = allocation_malloc(1, sizeof(VERTEX));
	if (v == NULL) {
	  return NULL;
	}
	v->index = -1;
	v->not_hydrogen.index = -1;
	for (i=0; i<DIMENSIONS; i++) {
		v->not_hydrogen.coord[i] = 0.0;
	}
	v->n_hydrogens = 0;
	v->hydrogens = NULL;
	return v;
}

int initialize_hydrogens(VERTEX *v, int n_hydrogens) {
	int i,j;
	if (v->hydrogens != NULL) {
		return FAIL;
	}
	v->hydrogens = allocation_malloc(n_hydrogens, sizeof(ATOM));
	if (v->hydrogens == NULL) {
	  return FAIL;
	}
	for (i=0; i<n_hydrogens; i++) {
		v->hydrogens[i].index = -1;
		for (j=0; j<DIMENSIONS; j++) {
			v->hydrogens[i].coord[j] = 0.0;
		}
	}
	v->n_hydrogens = n_hydrogens;
	return SUCCESS;
}

int delete_hydrogens(VERTEX *v) {
	if (v == NULL) {
		return FAIL;
	}
	if (v->n_hydrogens > 0) {
		free(v->hydrogens);
		v->hydrogens = NULL;
		v->n_hydrogens = 0;
	} else {
		return FAIL;
	}
	return SUCCESS;
}

int get_index(VERTEX *v) {
	return v->not_hydrogen.index;
}

char* print_vertex(VERTEX *v) {
  char *text = allocation_malloc(PRINT_LENGTH, sizeof(char));
  sprintf(text, "%d", v->not_hydrogen.index);
  return text;
}

void set_index(VERTEX *v, int ind) {
	v->index = ind;
}
