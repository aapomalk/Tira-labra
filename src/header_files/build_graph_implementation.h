#ifndef BUILD_GRAPH_IMPLEMENTATION_H
#define BUILD_GRAPH_IMPLEMENTATION_H

#include "graph.h"
#include "graph_implementation.h"


/* 
   The vertex_definitions is a list of lists.

   -Each list has as the first string residue identifier
   (either only resname or resname and segname or even resname, segname and resid).
   -In case if the same residue contains several vertex:
   before resname there is a number of vertexes still coming after the current vertex
   (first vertex of the residue has the largest number and last has always zero).
   -An example would be: "0 HSD", "0 HSD PROA" or "0 HSD PROA 12"

   -The rest are the atoms, one non-hydrogen and others are hydrogens.
   -Non-hydrogen has identifier 1, like: "1 ND1" (atom-name can be four letters long).
   -Hydrogens have identifier 2, like: "2 HD1".
   -The atoms need to be in the order they appear in the pdb.

   -An example list would be: "0 HSD", "1 ND1", "2 HD1" and n_of_hydrogens would be 1.
   -The first matching list is always used, 
   so index and segname definitions should be always before the general definitions.
 */
int read_pdb(char *pdb, char ***vertex_definitions, int *n_of_hydrogens, int n_of_definitions, GRAPH *g);
int compare_substring(char * line, int start_index, int number_of_chars_to_compare, char * compare);
int compare_definition_and_line(char * line, char * definition);
int compare_atomname_and_line(char * line, char * atomname);
void insert_atom(ATOM *a, char *line, int index);
int is_an_atom(char *line, int *index);


/* 
   Distance, angle and domain decomposition are defined in param -file.
   And the vertex definitions of course.
*/
int build_vertex_definitions_and_prepare_graph(char *param, GRAPH *g,
											   char ****vertex_definitions,
											   int **n_of_hydrogens,
											   int *n_of_definitions);
int compare_keyword_and_line(char *line, char *keyword);
void handle_n_of_definitions(char *line, int *n_of_definitions,
							 int **n_of_hydrogens,
							 char ****vertex_definitions);
void handle_angle(char *line, GRAPH *g);
void handle_distance(char *line, GRAPH *g);
void handle_domain_decomposition(char *line, GRAPH *g);
void handle_n_of_hydrogens(char *line, int **n_of_hydrogens,
						   char ****vertex_definitions, int index);
void malloc_and_copy(char *line, char ****vertex_definitions, int index,
					 int index2, int malloc_size);
void handle_residue_definition(char *line, char ****vertex_definitions,
							   int index);
void handle_atomname_definition(char *line, char ****vertex_definitions,
								int index, int index2);
void free_vert_def_n_hydr(char ***vertex_definitions, int *n_of_hydrogens,
						  int n_def);

int read_first_xtc(char *xtc, GRAPH *g);
int read_next_xtc(GRAPH *g);
int close_xtc();

#endif
