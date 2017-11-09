#include "graph.h"
#include "graph_implementation.h"
#include "constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

VERTEX * new_vertex() {
	int i;
	VERTEX *v = malloc(sizeof(VERTEX));
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
	v->hydrogens = malloc(n_hydrogens*sizeof(ATOM));
	for (i=0; i<n_hydrogens; i++) {
		v->hydrogens[i].index = -1;
		for (j=0; j<DIMENSIONS; j++) {
			v->hydrogens[i].coord[j] = 0.0;
		}
	}
	v->n_hydrogens = n_hydrogens;
	return SUCCESS;
}

int delete_vertex(VERTEX *v) {
	if (v == NULL) {
		return FAIL;
	}
	if (v->hydrogens != NULL) {
		free(v->hydrogens);
	}
	free(v);
	return SUCCESS;
}

double heuristic(VERTEX *a, VERTEX *b) {
	double x = 0;
	int i;
	for (i=0; i<DIMENSIONS; i++) {
		x += pow(a->not_hydrogen.coord[i] - b->not_hydrogen.coord[i], 2);
	}
	return sqrt(x);
}