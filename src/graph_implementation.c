#include "graph.h"
#include "graph_implementation.h"
#include "constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PRINT_LENGTH 30

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

int delete_vertex(VERTEX **v) {
	if (*v == NULL) {
		return FAIL;
	}
	if ((*v)->hydrogens != NULL) {
		free((*v)->hydrogens);
	}
	free(*v);
	*v = NULL;
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

int get_index(VERTEX *v) {
	return v->not_hydrogen.index;
}

char* print_vertex(VERTEX *v) {
	char *text = malloc(PRINT_LENGTH*sizeof(char));
	sprintf(text, "%d", v->not_hydrogen.index);
	return text;
}

BOX * new_box() {
	int i,j;
	BOX * box = malloc(sizeof(BOX));
	for (i=0; i<DIMENSIONS; i++) {
		box->decomposition[i] = 0;
		for (j=0; j<DIMENSIONS; j++) {
			box->vectors[i][j] = 0.0;
		}
	}
	box->domains = NULL;
	return box;
}

int number_of_domains(BOX *box) {
	int i = 1,j;
	for (j=0; j<DIMENSIONS; j++) {
		i = i * box->decomposition[j];
	}
	return i;
}

int delete_box(BOX **box) {
	int i;
	if (*box == NULL) {
		return FAIL;
	}
	if ((*box)->domains != NULL) {
		for (i=0; i<number_of_domains(*box); i++) {
			free((*box)->domains[i]);
		}
		free((*box)->domains);
	}
	free(*box);
	*box = NULL;
	return SUCCESS;
}

GRAPH * new_graph() {
	GRAPH * g = malloc(sizeof(GRAPH));
	g->number_of_nodes = 0;
	g->nodes = NULL;
	g->edges = NULL;
	g->box = new_box();
	return g;
}

int delete_graph(GRAPH **g) {
	int i,j;
	if (*g == NULL) {
		return FAIL;
	}
	if ((*g)->nodes != NULL) {
		VERTEX * v = (*g)->nodes;
		for (i=0; i<size_of_graph(*g); i++) {
			if (delete_vertex(&v) == FAIL) {
				return FAIL;
			}
			v++;
		}
		free((*g)->nodes);
	}
	if ((*g)->edges != NULL) {
		for (i=0; i<size_of_graph(*g); i++) {
			j=0;
			while ((*g)->edges[j] != NULL) {
				free((*g)->edges[j]);
				j++;
			}
		}
	}
	if (delete_box(&((*g)->box)) == FAIL) {
		return FAIL;
	}
	free(*g);
	*g = NULL;
	return SUCCESS;
}

int size_of_graph(GRAPH *g) {
	return g->number_of_nodes;
}