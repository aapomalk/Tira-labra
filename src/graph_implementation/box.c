#include "graph.h"
#include "graph_implementation.h"
#include "constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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