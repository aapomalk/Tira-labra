#include "graph.h"
#include "graph_implementation.h"
#include "constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
	int index = v->index;
	int *neighbours = malloc(DIMENSIONS * sizeof(int));
	int found = 1, i, level;
	int *checked = calloc(number_of_domains(g->box), sizeof(int));
	
	for (level=0; found == 1; level++) { /* current domain is on level 0, 
	level 1 is all the surrounding domains and so on */
		int value = 1 + 2 * level, next, equals_value = 0;
		found = 0;
		for (i=0; i<pow(value, DIMENSIONS); i++) {
			int j = i, k;
			for (k=0; k<DIMENSIONS; k++) {
				int a = -level + (j % value);
				neighbours[k] = a;
				if (a == value || a == -value) {
					equals_value = 1; /* at least one of the values need to be the level (+-) */
				}
				j /= value;
			}
			if (equals_value == 0) {
				continue; /* this is not on the right level */
			}
			next = get_neighbouring_domain_index(g->box, v->not_hydrogen.coord, neighbours);
			if (domain_is_within_reach(g, v->not_hydrogen.coord, neighbours) == 1 && checked[next] == 0) {
				int j;
				found = 1; /* there was still at least one domain reachable and unvisited 
				-> we can continue level-loop deeper */
				checked[next] = 1; /* we're not going to visit this domain again */
				for (j=0; j < g->box->n_of_vertex_in_domains[next]; j++) {
					VERTEX *x = &(g->box->domains[next][j]);
					double weight = is_connection(v, x, g);
					if (weight >= 0) {
						add_edge(index, x, weight, g);
					}
				}
			}
		}
	}
	
	free(checked);
	free(neighbours);
	return;
}

void add_edge(int index, VERTEX *v, double weight, GRAPH *g) {
	if (g->edges[index] == NULL) {
		g->edges[index] = malloc(INITIAL_EDGE_NUMBER * sizeof(EDGE));
		g->size_of_edge_lists[index] = INITIAL_EDGE_NUMBER;
		g->n_of_edges[index] = 0;
	}
	while (g->n_of_edges[index] >= g->size_of_edge_lists[index]) {
		g->size_of_edge_lists[index] *= 2;
		g->edges[index] = realloc(g->edges[index], g->size_of_edge_lists[index] * sizeof(EDGE));
	}
	g->n_of_edges[index] += 1;
	g->edges[index][g->n_of_edges[index]].weight = weight;
	g->edges[index][g->n_of_edges[index]].node = v;
}

/* returns the distance if success and negative value otherwise */
double is_connection(VERTEX *a, VERTEX *b, GRAPH *g) {
	double *vec = malloc(DIMENSIONS * sizeof(double));
	double distance = heuristic2(a, b, g, vec);
	if (distance > g->distance) {
		return -1.0;
	}
	if (minimum_angle_between(a, b, g, vec, distance) > g->angle) {
		return -1.0;
	}
	return distance;
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

double heuristic(VERTEX *a, VERTEX *b, GRAPH *g) {
	double *x = malloc(DIMENSIONS * sizeof(double));
	double ret = heuristic2(a, b, g, x);
	free(x);
	return ret;
}

double heuristic2(VERTEX *a, VERTEX *b, GRAPH *g, double *from_a_to_b) {
	double x = 0, min = -1.0;
	int i;
	if (g == NULL || g->box->domains == NULL) { /* in these cases direct distance is used */
		for (i=0; i<DIMENSIONS; i++) {
			x += pow(a->not_hydrogen.coord[i] - b->not_hydrogen.coord[i], 2);
		}
	} else { /* otherwise periodicity is used */
		for (i=0; i<pow(3,DIMENSIONS); i++) { /* DIMENSIONS = 3 -> 3^3=27 periodic images are checked */
			int m,n;
			double copy[DIMENSIONS];
			x = 0.0;
			for (m=0; m<DIMENSIONS; m++) {
				double b_vec = 0.0;
				int j=i;
				for (n=0; n<DIMENSIONS; n++) {
					int k = -1 + (j % 3); /* each dimension gets -1, 0 and 1 */
					b_vec += k * g->box->vectors[n][m];
					j /= 3; /* the remainder gets removed in integer division */
				}
				copy[m] = a->not_hydrogen.coord[m] - b->not_hydrogen.coord[m] + b_vec;
				x += pow(copy[m], 2);
			}
			if (min < 0 || x < min) {
				min = x;
				for (m=0; m<DIMENSIONS; m++) {
					from_a_to_b[m] = copy[m];
				}
			}
		}
		x = min; /* finally the smallest periodic distances is returned */
	}
	return sqrt(x);
}

double minimum_angle_between(VERTEX *a, VERTEX *b, GRAPH *g, double *vec, double distance) {
	int i,j;
	double min = -1;
	for (j=0; j < a->n_hydrogens; j++) {
		double length1 = 0;
		double dot_product = 0;
		double value;
		for (i=0; i<DIMENSIONS; i++) {
			double vec1 = a->hydrogens[j].coord[i] - a->not_hydrogen.coord[i];
			dot_product += vec1 * vec[i];
			length1 += vec1 * vec1;
		}
		value = acos(dot_product / sqrt(length1 * distance)) * 180.0 / PI;
		if (min < 0 || value < min) {
			min = value;
		}
	}
	return min;
}

