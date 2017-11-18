#include "unity.h" /* I have downloaded unity from http://www.throwtheswitch.org/unity/ */
#include "../src/constants.h"
#include "../src/graph.h"
#include "../src/graph_implementation.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define TEST_GRAPH_INDEX(g) do { \
	int i; \
	for (i=0; i<(g)->number_of_nodes; i++) { \
		TEST_ASSERT_EQUAL_INT(i, (g)->nodes[i].index); \
	} \
} while (0) /* note that g needs to be a GRAPH */

void test_vertex(void) {
	int i;
	VERTEX *v;
	v = new_vertex();
	TEST_ASSERT_EQUAL_INT(-1, v->index);
	TEST_ASSERT(NULL == v->hydrogens);
	TEST_ASSERT_EQUAL_INT(-1, v->not_hydrogen.index);
	for (i=0; i<3; i++) {
		TEST_ASSERT_EQUAL_FLOAT(0.0, v->not_hydrogen.coord[0]);
	}
	i=3;
	TEST_ASSERT_EQUAL_INT(SUCCESS, initialize_hydrogens(v, i));
	TEST_ASSERT_EQUAL_INT(i, v->n_hydrogens);
	TEST_ASSERT_EQUAL_INT(-1, get_index(v));
	TEST_ASSERT_EQUAL_STRING("-1", print_vertex(v));
	TEST_ASSERT_EQUAL_INT(SUCCESS, delete_hydrogens(v));
	TEST_ASSERT_EQUAL_INT(FAIL, delete_hydrogens(v));
	free(v); /* if a vertex is not inserted in graph it needs to be freed */
}

/* first testing without periodicity */
void test_heuristic(void) {
	VERTEX *a = new_vertex();
	VERTEX *b = new_vertex();
	a->not_hydrogen.coord[0] = 1.0;
	b->not_hydrogen.coord[0] = 2.0;
	TEST_ASSERT_EQUAL_FLOAT(1.0, heuristic(a, b, NULL)); 
	a->not_hydrogen.coord[1] = 1.0;
	TEST_ASSERT_EQUAL_FLOAT(sqrt(2.0), heuristic(a, b, NULL));
	free(a);
	free(b);
}

void test_graph(void) {
	GRAPH *g = new_graph();
	TEST_ASSERT_EQUAL_INT(0, size_of_graph(g));
	TEST_ASSERT_EQUAL_INT(INITIAL_LENGTH, g->list_length);
	TEST_ASSERT(NULL != g->box);
	TEST_ASSERT(NULL != g->nodes);
	TEST_ASSERT(NULL != g->edges);
	TEST_ASSERT_EQUAL_INT(0, number_of_domains(g->box));
	TEST_ASSERT_EQUAL_INT(SUCCESS, delete_graph(&g));
	TEST_ASSERT(NULL == g);
}

void test_vertex_graph(void) {
	VERTEX *a = new_vertex();
	VERTEX *b = new_vertex();
	VERTEX *c = new_vertex();
	GRAPH *g = new_graph();
	a->not_hydrogen.index = 4;
	a->not_hydrogen.coord[0] = 3.0;
	initialize_hydrogens(a, 1);
	a->hydrogens[0].coord[0] = 8.0;
	TEST_ASSERT_EQUAL_FLOAT(8.0, a->hydrogens[0].coord[0]);
	b->not_hydrogen.index = 3;
	b->not_hydrogen.coord[0] = 5.0;
	c->not_hydrogen.index = 1;
	TEST_ASSERT_EQUAL_FLOAT(3.0, a->not_hydrogen.coord[0]);
	TEST_ASSERT_EQUAL_INT(-1, a->index); /* original index outside of g */
	TEST_ASSERT_EQUAL_INT(SUCCESS, add_vertex(g, a));
	TEST_ASSERT_EQUAL_FLOAT(8.0, g->nodes[0].hydrogens[0].coord[0]); /* a (note that a doesn't exist outside of g anymore) */
	TEST_GRAPH_INDEX(g); /* tests that the indexes are correct */
	TEST_ASSERT_EQUAL_FLOAT(3.0, g->nodes[0].not_hydrogen.coord[0]); /* a */
	TEST_ASSERT_EQUAL_INT(SUCCESS, add_vertex(g, b));
	TEST_GRAPH_INDEX(g);
	TEST_ASSERT_EQUAL_INT(1, g->nodes[1].index); /* a, notice the changed index */
	TEST_ASSERT_EQUAL_FLOAT(8.0, g->nodes[1].hydrogens[0].coord[0]); /* a */
	TEST_ASSERT_EQUAL_FLOAT(3.0, g->nodes[1].not_hydrogen.coord[0]); /* a */
	TEST_ASSERT_EQUAL_FLOAT(5.0, g->nodes[0].not_hydrogen.coord[0]); /* b */
	add_vertex(g, c);
	TEST_GRAPH_INDEX(g);
	TEST_ASSERT_EQUAL_FLOAT(3.0, g->nodes[2].not_hydrogen.coord[0]); /* a */
	delete_graph(&g);
}

void test_insert_many_vertex_to_graph(void) {
	int i,j=INITIAL_LENGTH;
	GRAPH *g = new_graph();
	for (i=0; i<100; i++) {
		VERTEX *v = new_vertex();
		TEST_ASSERT_EQUAL_INT(i, size_of_graph(g));
		TEST_ASSERT_EQUAL_INT(SUCCESS, add_vertex(g, v));
		if (i >= j) {
			j = j * 2;
		}
		TEST_ASSERT_EQUAL_INT(j, g->list_length);
	}
	delete_graph(&g);
}

void test_box_preparation(void) {
	int i,j,k,decomposition[DIMENSIONS] = {5, 3, 4};
	COORDINATE vectors[DIMENSIONS];
	GRAPH *g = new_graph();
	for (i=0; i<DIMENSIONS; i++) {
		for (j=0; j<DIMENSIONS; j++) {
			vectors[i][j] = 0.0;
		}
	}
	vectors[0][0] = 5.0;
	vectors[1][1] = 3.0; /* these vectors are orthogonal */
	vectors[2][2] = 4.0;
	
	for (i=0; i<decomposition[0]; i++) {
		for (j=0; j<decomposition[1]; j++) {
			for (k=0; k<decomposition[2]; k++) {
				VERTEX *v = new_vertex();
				v->not_hydrogen.coord[0] = 0.5 + i;
				v->not_hydrogen.coord[1] = 0.5 + j;
				v->not_hydrogen.coord[2] = 0.5 + k;
				TEST_ASSERT_EQUAL_INT(SUCCESS, add_vertex(g, v));
			}
		}
	}
	prepare_box(g, decomposition, vectors);
	TEST_ASSERT_EQUAL_INT(5*3*4 ,number_of_domains(g->box));
	for (i=0; i<5*4*3; i++) {
		TEST_ASSERT_EQUAL_INT(1, g->box->n_of_vertex_in_domains[i]); /* each domain has one vertex */
		TEST_ASSERT_EQUAL_INT(3, g->box->size_of_domains[i]);
		g->nodes[i].not_hydrogen.coord[0] = 0.5;
		g->nodes[i].not_hydrogen.coord[1] = 0.5;
		g->nodes[i].not_hydrogen.coord[2] = 0.5;
	}
	prepare_box(g, decomposition, vectors);
	TEST_ASSERT_EQUAL_INT(5*4*3, g->box->n_of_vertex_in_domains[0]); /* all nodes are in one domain */
	for (i=1; i<5*4*3; i++) {
		TEST_ASSERT_EQUAL_INT(0, g->box->n_of_vertex_in_domains[i]); /* others are empty */
		TEST_ASSERT_EQUAL_INT(3, g->box->size_of_domains[i]);
	}
	delete_graph(&g);
}

void test_periodicity(void) {
	VERTEX *a = new_vertex();
	VERTEX *b = new_vertex();
	int i,j,decomposition[DIMENSIONS] = {1, 1, 1}; /* decomposition is not needed, but we need at least one domain */
	COORDINATE vectors[DIMENSIONS];
	GRAPH *g = new_graph();
	for (i=0; i<DIMENSIONS; i++) {
		for (j=0; j<DIMENSIONS; j++) {
			vectors[i][j] = 0.0;
		}
	}
	vectors[0][0] = 3.0;
	vectors[1][1] = 3.0; /* our box is a cube */
	vectors[2][2] = 3.0;
	
	a->not_hydrogen.coord[0] = 0.4;
	b->not_hydrogen.coord[0] = 2.9;
	
	add_vertex(g, a);
	add_vertex(g, b);
	
	prepare_box(g, decomposition, vectors);
	TEST_ASSERT_EQUAL_FLOAT(2.5, heuristic(&(g->nodes[0]), &(g->nodes[1]), NULL)); /* without periodicity */
	TEST_ASSERT_EQUAL_FLOAT(0.5, heuristic(&(g->nodes[0]), &(g->nodes[1]), g)); /* with periodicity */
	
	delete_graph(&g);
}

void test_domain_within_reach(void) {
	GRAPH *g = new_graph();
	int i,j,decomposition[DIMENSIONS] = {10, 10, 10};
	COORDINATE vectors[DIMENSIONS], coord;
	int *neighbour = malloc(DIMENSIONS * sizeof(int));
	int a,b,c;
	
	for (i=0; i<DIMENSIONS; i++) {
		for (j=0; j<DIMENSIONS; j++) {
			vectors[i][j] = 0.0;
		}
	}
	vectors[0][0] = 10.0;
	vectors[1][1] = 10.0; /* our box is a cube */
	vectors[2][2] = 10.0;
	set_distance(g, 1.0); /* this determines whether domain is within reach */
	prepare_box(g, decomposition, vectors);
	
	for (a=0; a<DIMENSIONS; a++) {
		coord[a] = 5.5;
	}
	
	for (a=0; a<pow(5, DIMENSIONS); a++) {
		int true = 1;
		char message[30];
		b=a;
		for (c=0; c<DIMENSIONS; c++) {
			int x = -2 + (b % 5);
			if (x < -1 || x > 1) { /* only the closest domains are within reach */
				true = 0;
			}
			neighbour[c] = x;
			b /= 5;
		}
		sprintf(message, "neighbour: %d %d %d", neighbour[0], neighbour[1], neighbour[2]);
		TEST_ASSERT_EQUAL_INT_MESSAGE(true, domain_is_within_reach(g, coord, neighbour), message);
	}
	
	free(neighbour);
	delete_graph(&g);
}

void test_domain_origin(void) {
	GRAPH *g = new_graph();
	int i,j,decomposition[DIMENSIONS] = {10, 10, 10};
	COORDINATE vectors[DIMENSIONS], coord, *ret;
	
	for (i=0; i<DIMENSIONS; i++) {
		for (j=0; j<DIMENSIONS; j++) {
			vectors[i][j] = 0.0;
		}
	}
	vectors[0][0] = 10.0;
	vectors[1][1] = 10.0; /* our box is a cube */
	vectors[2][2] = 10.0;
	prepare_box(g, decomposition, vectors);
	
	for (i=0; i<DIMENSIONS; i++) {
		coord[i] = 1.5;
	}
	ret = get_domain_origin(g->box, coord);
	for (i=0; i<DIMENSIONS; i++) {
		TEST_ASSERT_EQUAL_FLOAT(1.0, (*ret)[i]);
	}
	free(ret);
	
	delete_graph(&g);
}

void test_get_edges(void) {
	GRAPH *g = new_graph();
	int i,j,k,decomposition[DIMENSIONS] = {10, 10, 10};
	COORDINATE vectors[DIMENSIONS];
	
	for (i=0; i<DIMENSIONS; i++) {
		for (j=0; j<DIMENSIONS; j++) {
			vectors[j][i] = 0.0;
		}
	}
	vectors[0][0] = 10.0;
	vectors[1][1] = 10.0; /* our box is a cube */
	vectors[2][2] = 10.0;
	for (i=0; i<decomposition[0]; i++) {
		for (j=0; j<decomposition[1]; j++) {
			for (k=0; k<decomposition[2]; k++) {
				int a,b;
				VERTEX *v = new_vertex();
				v->not_hydrogen.coord[0] = 0.5 + i;
				v->not_hydrogen.coord[1] = 0.5 + j;
				v->not_hydrogen.coord[2] = 0.5 + k;
				initialize_hydrogens(v, 6);
				for (a=0; a<6; a++) {
					for (b=0; b<DIMENSIONS; b++) {
						v->hydrogens[a].coord[b] = v->not_hydrogen.coord[b];
					}
				}
				v->hydrogens[0].coord[0] += 0.5;
				v->hydrogens[1].coord[0] -= 0.5;
				v->hydrogens[2].coord[1] += 0.5;
				v->hydrogens[3].coord[1] -= 0.5;
				v->hydrogens[4].coord[2] += 0.5;
				v->hydrogens[5].coord[2] -= 0.5;
				add_vertex(g, v);
			}
		}
	}
	prepare_box(g, decomposition, vectors);
	set_distance(g, 2.3);
	set_angle(g, 0.0001);
	for (i=0; i<size_of_graph(g); i++) {
		EDGE *e;
		char message[30];
		sprintf(message, "i: %d", i);
		TEST_ASSERT_EQUAL_INT(-1, g->n_of_edges[i]);
		e = get_edges(g, get_vertex(g, i));
		for (j=0; j<g->n_of_edges[i]; j++) {
			for (k=0; k<DIMENSIONS; k++) {
				printf("%f ", e[j].node->not_hydrogen.coord[k]);
			}
			printf("\n");
		}
		TEST_ASSERT_EQUAL_INT_MESSAGE(12, g->n_of_edges[i], message);
		TEST_ASSERT(NULL != e);
	}
	delete_graph(&g);
}

int main(void) {
	printf("\n");
	UNITY_BEGIN();
	RUN_TEST(test_domain_origin);
	RUN_TEST(test_get_edges);
	RUN_TEST(test_vertex);
	RUN_TEST(test_heuristic);
	RUN_TEST(test_graph);
	RUN_TEST(test_vertex_graph);
	RUN_TEST(test_insert_many_vertex_to_graph);
	RUN_TEST(test_box_preparation);
	RUN_TEST(test_periodicity);
	RUN_TEST(test_domain_within_reach);
	return UNITY_END();
}