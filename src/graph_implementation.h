#ifndef GRAPH_IMPLEMENTATION_H
#define GRAPH_IMPLEMENTATION_H

#define DIMENSIONS 3

typedef double COORDINATE [DIMENSIONS];

typedef struct ATOM {
	COORDINATE coord;
	int index;
} ATOM;

/* typedef in graph.h */
struct VERTEX {
	int index; /* the index of GRAPH */
	ATOM not_hydrogen;
	int n_hydrogens;
	ATOM *hydrogens;
};

int initialize_hydrogens(VERTEX *v, int n_hydrogens);

typedef struct BOX {
	int decomposition [3]; /* how many boxes along vector 1, 2 and 3 */
	COORDINATE vectors [3]; /* these define the BOX shape */
	VERTEX *** domains; /* 3D list of domains shaped like box */
} BOX;

/* typedef in graph.h */
struct GRAPH {
	int number_of_nodes;
	VERTEX *nodes; /* the nodes themselves */
	EDGE **edges; /* a (null-terminated) list of edges for each node */
	BOX box; /* the domain decomposition of graph */
};

/* returns a list of domains reachable from coordinate c by distance */
VERTEX ** get_neighbours(BOX *b, COORDINATE c, double distance);

#endif