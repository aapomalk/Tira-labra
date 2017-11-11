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
	box->n_of_vertex_in_domains = NULL;
	box->size_of_domains = NULL;
	return box;
}

int number_of_domains(BOX *box) {
	int i = 1,j;
	for (j=0; j<DIMENSIONS; j++) {
		i = i * box->decomposition[j];
	}
	return i;
}

void free_domains(BOX *box) {
	int i;
	if (box->domains != NULL) {
		for (i=0; i<number_of_domains(box); i++) {
			free(box->domains[i]);
		}
		free(box->domains);
		free(box->n_of_vertex_in_domains);
		free(box->size_of_domains);
	}
}

int delete_box(BOX **box) {
	if (*box == NULL) {
		return FAIL;
	}
	free_domains(*box);
	free(*box);
	*box = NULL;
	return SUCCESS;
}

int * get_domain_index(BOX *b, COORDINATE c) {
	int *i = malloc(DIMENSIONS * sizeof(int));
	return i; /* finish this later */
}

void prepare_box(GRAPH *g, int *decomposition, COORDINATE *vectors) {
	BOX *box = g->box;
	int i,j,previous = number_of_domains(box), current;
	for (i=0; i<previous; i++) {
		free(box->domains[i]);
		box->n_of_vertex_in_domains[i] = 0;
		box->size_of_domains = 0;
	}
	for (i=0; i<DIMENSIONS; i++) {
		box->decomposition[i] = decomposition[i];
		for (j=0; j<DIMENSIONS; j++) {
			box->vectors[i][j] = vectors[i][j];
		}
	}
	current = number_of_domains(box);
	if (box->domains == NULL) {
		box->domains = malloc(current * sizeof(VERTEX*));
	} else {
		box = realloc(box, current * sizeof(VERTEX*));
	}
}

GRAPH * new_graph() {
	int initial_length = INITIAL_LENGTH;
	GRAPH * g = malloc(sizeof(GRAPH));
	g->number_of_nodes = 0;
	g->list_length = initial_length;
	g->nodes = malloc(initial_length * sizeof(VERTEX));
	g->edges = malloc(initial_length * sizeof(EDGE*));
	g->box = new_box();
	return g;
}

int delete_graph(GRAPH **g) {
	int i,j;
	if (*g == NULL) {
		return FAIL;
	}
	if ((*g)->nodes != NULL) {
		for (i=size_of_graph(*g)-1; i>=0; i--) {
			VERTEX *v = (*g)->nodes;
			v += i;
			if (delete_hydrogens(v) == FAIL) {
				return FAIL;
			}
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

VERTEX * get_vertex(GRAPH *g, int index) {
	VERTEX *v = g->nodes;
	v += index;
	return v;
}

/* assuming that the vertex index are in order */
VERTEX * find_vertex(GRAPH *g, int index) {
	int i=0,j=size_of_graph(g)-1;
	VERTEX *v;
	while (j > i) {
		int k = (j - i) / 2 + i, x;
		v = get_vertex(g, k);
		x = get_index(v);
		if (x == index) {
			return v;
		} else if (x < index) {
			j = x-1;
		} else {
			i = x+1;
		}
	}
	return NULL;
}

EDGE * get_edges(GRAPH *g, VERTEX *v) {
	int i = v->index;
	EDGE *e = g->edges[i];
	if (e != NULL) {
		return e;
	}
	form_edges(g, v);
	return e;
}

void form_edges(GRAPH *g, VERTEX *v) {
	int i = v->index;
	EDGE *e = g->edges[i];
	if (e != NULL) {
		return;
	}
	return; /* complete this later */
}

int add_vertex(GRAPH *g, VERTEX *v) {
	int number = g->number_of_nodes;
	int index = number - 1;
	VERTEX *n = get_vertex(g, index);
	if (number >= g->list_length - 1) {
		g->list_length = 2 * g->list_length;
		g->nodes = realloc(g->nodes, g->list_length * sizeof(VERTEX));
		g->edges = realloc(g->edges, g->list_length * sizeof(EDGE*));
	}
	set_index(v, 0);
	g->nodes[number] = *v; /* contents of v are copied */
	g->edges[number] = NULL;
	g->number_of_nodes++;
	while (index >= 0 && get_index(n) > get_index(v)) {
		set_index(n, index+1);
		g->nodes[index+1] = *n;
		set_index(v, index);
		g->nodes[index] = *v;
		g->edges[index+1] = g->edges[index];
		g->edges[index] = NULL;
		index--;
		n = get_vertex(g, index);
	}
	v->hydrogens = NULL; /* this prevents the access of hydrogens through v */
	free(v); /* the vertex is copied to graph and is not needed outside anymore
	(note that free might not clear the contents of v) */
	return SUCCESS;
}

void set_index(VERTEX *v, int ind) {
	v->index = ind;
}