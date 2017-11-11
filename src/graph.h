#ifndef GRAPH_H
#define GRAPH_H

typedef struct VERTEX VERTEX;

VERTEX * new_vertex();
double heuristic(VERTEX *a, VERTEX *b);
int get_index(VERTEX *v); /* index to identify v, see find_vertex */
char* print_vertex(VERTEX *v);

typedef struct EDGE {
	double weight;
	VERTEX *node; /* pointer to node that the edge is pointing towards */
} EDGE;

typedef struct GRAPH GRAPH;

GRAPH * new_graph();
int delete_graph(GRAPH **g); /* pass the address of pointer g like &g */
int size_of_graph(GRAPH *g);
VERTEX * get_vertex(GRAPH *g, int index); /* 0 <= index < size_of_graph */
VERTEX * find_vertex(GRAPH *g, int index); /* index might be larger than size_of_graph */
int add_vertex(GRAPH *g, VERTEX *v);

/* returns the (null-terminated) list of edges (weights and neighbours) */
EDGE * get_edges(GRAPH *g, VERTEX *v);

#endif