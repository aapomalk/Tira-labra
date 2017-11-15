#include "graph.h"
#include "graph_implementation.h"
#include "constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* this is now old file, the new files are in graph_implementation folder */

#define ABS(X) ((X)<0 ? -(X) : (X))

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

double heuristic(VERTEX *a, VERTEX *b, GRAPH *g) {
	double x = 0, min = -1.0;
	int i;
	if (g == NULL || g->box->domains == NULL) { /* in these cases direct distance is used */
		for (i=0; i<DIMENSIONS; i++) {
			x += pow(a->not_hydrogen.coord[i] - b->not_hydrogen.coord[i], 2);
		}
	} else { /* otherwise periodicity is used */
		for (i=0; i<pow(3,DIMENSIONS); i++) { /* DIMENSIONS = 3 -> 3^3=27 periodic images are checked */
			int m,n;
			x = 0.0;
			for (m=0; m<DIMENSIONS; m++) {
				double b_vec = 0.0;
				int j=i;
				for (n=0; n<DIMENSIONS; n++) {
					int k = -1 + (j % 3); /* each dimension gets -1, 0 and 1 */
					b_vec += k * g->box->vectors[n][m];
					j /= 3; /* the remainder gets removed in integer division */
				}
				x += pow(a->not_hydrogen.coord[m] - b->not_hydrogen.coord[m] + b_vec, 2);
			}
			if (min < 0 || x < min) {
				min = x;
			}
		}
		x = min; /* finally the smallest periodic distances is returned */
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

/* 
   In gromacs the first box vector is parallel to x-axis,
   the second box vector lies in xy-plane (not parallel to x-axis)
   and the third is somewhere above the xy-plane.
*/
int get_domain_index(BOX *b, COORDINATE c) {
	int *i = malloc(DIMENSIONS * sizeof(int));
	int j,x=0,y=1;
	
	i = get_domain_indexes(b, c, i);
	
	for (j=0; j<DIMENSIONS; j++) {
		x += y * i[j];
		y *= b->decomposition[j];
	}
	free(i);
	return x;
}

int * get_domain_indexes(BOX *b, COORDINATE c, int *i) {
	int j,k;
	COORDINATE copy;
	for (j=0; j<DIMENSIONS; j++) {
		copy[j] = c[j]; /* it's important not to modify c */
	}
	/* it's important to start from the last coordinate,
	that way we can remove its effect from c and move to lower dimensional vectors */
	for (j=DIMENSIONS-1; j>=0; j--) { 
		double d = copy[j] / b->vectors[j][j];
		i[j] = (int) floor(b->decomposition[j] * d);
		for (k=0; k<DIMENSIONS; k++) {
			copy[k] = copy[k] - d * b->vectors[j][k]; /* removing the (j+1)'th dimension */
		}
		while (i[j] >= b->decomposition[j]) {
			i[j] -= b->decomposition[j];
		}
		while (i[j] < 0) {
			i[j] += b->decomposition[j];
		}
	}
	return i;
}

int get_neighbouring_domain_index(BOX *b, COORDINATE c, int *x) {
	int i,j;
	for (i=0; i<DIMENSIONS; i++) {
		double d = 1.0 * x[i] / b->decomposition[i];
		for (j=0; j<DIMENSIONS; j++) {
			c[i] += d * b->vectors[i][j]; /* this is now based on coordinates, but this could be purely integer function */
		}
	}
	return get_domain_index(b, c);
}

COORDINATE * projection(COORDINATE a, COORDINATE b) {
	COORDINATE *c = malloc(sizeof(COORDINATE));
	int i;
	double length_of_b = 0;
	for (i=0; i<DIMENSIONS; i++) {
		length_of_b += b[i] * b[i];
	}
	length_of_b = sqrt(length_of_b);
	for (i=0; i<DIMENSIONS; i++) {
		(*c)[i] = (a[i] * b[i]) / length_of_b;
	}
	return c;
}

void prepare_box(GRAPH *g, int *decomposition, COORDINATE *vectors) {
	BOX *box = g->box;
	int i,j,previous = number_of_domains(box), current;
	for (i=0; i<DIMENSIONS; i++) {
		box->decomposition[i] = decomposition[i];
		for (j=0; j<DIMENSIONS; j++) {
			if (j > i) {
				box->vectors[i][j] = 0.0; /* the n'th vector will have n dimensions */
			} else {
				box->vectors[i][j] = vectors[i][j];
			}
		}
	}
	current = number_of_domains(box);
	if (box->domains == NULL) {
		box->domains = malloc(current * sizeof(VERTEX*));
		box->size_of_domains = malloc(current * sizeof(int));
		box->n_of_vertex_in_domains = malloc(current * sizeof(int));
		for (i=0; i<current; i++) {
			box->size_of_domains[i] = 0;
			box->n_of_vertex_in_domains[i] = 0;
		}
	} else {
		box->domains = realloc(box->domains, current * sizeof(VERTEX*));
		box->size_of_domains = realloc(box->size_of_domains, current * sizeof(int));
		box->n_of_vertex_in_domains = realloc(box->n_of_vertex_in_domains, current * sizeof(int));
		for (i=0; i<current; i++) {
			if (i >= previous) {
				box->size_of_domains[i] = 0;
			}
			box->n_of_vertex_in_domains[i] = 0;
		}
	}
	for (i=0; i<size_of_graph(g); i++) {
		VERTEX v = g->nodes[i];
		int index = get_domain_index(box, v.not_hydrogen.coord);
		g->n_of_edges[i] = -1;
		add_vertex_to_domain(index, &v, box);
	}
}

void add_vertex_to_domain(int domain, VERTEX *v, BOX *box) {
	int size = box->size_of_domains[domain];
	int number = box->n_of_vertex_in_domains[domain];
	if (size <= 0) {
		size = box->size_of_domains[domain] = INITIAL_DOMAIN_SIZE;
		box->domains[domain] = malloc(INITIAL_DOMAIN_SIZE * sizeof(VERTEX));
		number = box->n_of_vertex_in_domains[domain] = 0;
	}
	if (number >= size) {
		size = box->size_of_domains[domain] = 2 * size;
		box->domains[domain] = realloc(box->domains[domain], size * sizeof(VERTEX));
	}
	box->domains[domain][number] = *v;
	box->n_of_vertex_in_domains[domain] = number + 1;
}

GRAPH * new_graph() {
	int initial_length = INITIAL_LENGTH,i;
	GRAPH * g = malloc(sizeof(GRAPH));
	g->number_of_nodes = 0;
	g->list_length = initial_length;
	g->nodes = malloc(initial_length * sizeof(VERTEX));
	g->edges = malloc(initial_length * sizeof(EDGE*));
	g->box = new_box();
	g->n_of_edges = malloc(initial_length * sizeof(int));
	g->size_of_edge_lists = malloc(initial_length * sizeof(int));
	for (i=0; i<INITIAL_LENGTH; i++) {
		g->n_of_edges[i] = -1;
		g->size_of_edge_lists[i] = 0;
	}
	g->distance = ASSUMED_DISTANCE;
	g->angle = ASSUMED_ANGLE;
	return g;
}

/* max distance between nodes in order to be neighbours */
void set_distance(GRAPH *g, double distance) {
	if (distance > 0.0) {
		g->distance = distance;
	}
}

/* the max angle formed from the node main atoms (not_hydrogen) and one of the first nodes hydrogens */
void set_angle(GRAPH *g, double angle) {
	if (angle > 0 && angle <= 180) {
		g->angle = angle;
	}
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
	free((*g)->n_of_edges);
	free((*g)->size_of_edge_lists);
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
	if (g->n_of_edges[i] >= 0) {
		return e;
	}
	form_edges(g, v);
	return e;
}

void form_edges(GRAPH *g, VERTEX *v) {
	/*int i = v->index;
	EDGE *e = g->edges[i];*/
	
	return; /* complete this later */
}

int domain_is_within_reach(GRAPH *g, COORDINATE c, int *x) {
	double distance = g->distance;
	BOX *b = g->box;
	int i,j;
	COORDINATE *origin = get_domain_origin(b, c);
	for (i=0; i<DIMENSIONS; i++) {
		for (j=0; j<DIMENSIONS; j++) {
			(*origin)[i] += x[j] * b->vectors[j][i] / b->decomposition[j];
		}
	}
	for (i=0; i<DIMENSIONS; i++) {
		double min = (*origin)[i];
		double max = min;
		for (j=0; j<DIMENSIONS; j++) {
			if (b->vectors[j][i] > 0) {
				max += b->vectors[j][i] / b->decomposition[j];
			} else {
				min -= b->vectors[j][i] / b->decomposition[j];
			}
		}
		if (c[i] + distance < min || c[i] - distance > max) {
			return FAIL;
		}
	}
	
	free(origin);
	return SUCCESS;
}

COORDINATE * get_domain_origin(BOX *b, COORDINATE c) {
	int *i = malloc(DIMENSIONS * sizeof(int));
	COORDINATE *coord = malloc(sizeof(COORDINATE));
	int j,k;
	for (j=0; j<DIMENSIONS; j++) {
		(*coord)[j] = 0.0;
	}
	
	i = get_domain_indexes(b, c, i);
	for (j=0; j<DIMENSIONS; j++) {
		for (k=0; k<DIMENSIONS; k++) {
			(*coord)[j] += b->vectors[k][j] * i[k] / b->decomposition[k];
		}
	}
	
	return coord;
}

int add_vertex(GRAPH *g, VERTEX *v) {
	int number = g->number_of_nodes;
	int index = number - 1;
	VERTEX *n = get_vertex(g, index);
	if (number >= g->list_length) {
		int length,i;
		length = g->list_length = 2 * g->list_length;
		g->nodes = realloc(g->nodes, length * sizeof(VERTEX));
		g->edges = realloc(g->edges, length * sizeof(EDGE*));
		g->n_of_edges = realloc(g->n_of_edges, length * sizeof(int));
		g->size_of_edge_lists = realloc(g->size_of_edge_lists, length * sizeof(int));
		for (i=number; i<length; i++) {
			g->n_of_edges[i] = -1;
			g->size_of_edge_lists[i] = 0;
		}
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