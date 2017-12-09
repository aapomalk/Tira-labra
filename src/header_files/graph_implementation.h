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
int delete_hydrogens(VERTEX *v);
void set_index(VERTEX *v, int index); /* the graph index */



typedef struct BOX {
	int decomposition [DIMENSIONS]; /* how many boxes along vector 1, 2 and 3 */
	COORDINATE vectors [DIMENSIONS]; /* these define the BOX shape */
	VERTEX ** domains; /* list of domains shaped like box */
	int *n_of_vertex_in_domains;
	int *size_of_domains;
} BOX;
BOX * new_box();
int number_of_domains(BOX *box);
void free_domains(BOX *box);
int delete_box(BOX **box);
/* returns the domain of coordinate if x = {0, 0, 0} 
and next domain in y-direction if x = {0, 1, 0} etc. */
int get_neighbouring_domain_index(BOX *b, COORDINATE c, int *x); /* length of x is DIMENSIONS */
int get_domain_index(BOX *b, COORDINATE c);
int * get_domain_indexes(BOX *b, COORDINATE c, int *i);
COORDINATE * get_domain_origin(BOX *b, COORDINATE c);
void add_vertex_to_domain(int domain, VERTEX *v, BOX *box);



void prepare_box(GRAPH *g, int *decomposition, COORDINATE *vectors);
/* typedef in graph.h */
struct GRAPH {
	int number_of_nodes, list_length;
	VERTEX *nodes; /* the nodes themselves */
	int *n_of_edges,*size_of_edge_lists;
	double distance, angle;
	EDGE **edges; /* a (null-terminated) list of edges for each node */
	BOX *box; /* the domain decomposition of graph */
};
void set_distance(GRAPH *g, double distance);
void set_angle(GRAPH *g, double angle);
void form_edges(GRAPH *g, VERTEX *v);
double minimum_angle_between(VERTEX *a, VERTEX *b, GRAPH *g, double *vec, double distance);
int domain_is_within_reach(GRAPH *g, COORDINATE c, int *x);
double is_connection(VERTEX *a, VERTEX *b, GRAPH *g);
void add_edge(int index, VERTEX *v, double weight, GRAPH *g);
void set_vertex_list_length(GRAPH *g, int length);


double heuristic2(VERTEX *a, VERTEX *b, GRAPH *g, double *from_a_to_b);


void clear_edges(GRAPH *g);
COORDINATE * projection(COORDINATE a, COORDINATE b);

#endif
