#include "build_graph.h"
#include "build_graph_implementation.h"
#include "constants.h"
#include "graph.h"
#include "graph_implementation.h"
#include <stdio.h>
#include <stdlib.h>

#define LINE_LENGTH_BUFFER 100

#define KEYWORD_LENGTH 4
#define N_OF_DEFINITIONS "NDEF"
#define ANGLE "ANGL"
#define DISTANCE "DIST"
#define DOMAIN_DECOMPOSITION "DCMP"
#define N_OF_HYDROGENS "NHYD"
#define RESIDUE_DEFINITION "RESI"
#define ATOMNAME_DEFINITION "ATOM"

#define RESIDUE_DEFINITION_LENGTH 16
#define ATOMNAME_DEFINITION_LENGTH 8

int compare_keyword_and_line(char *line, char *keyword) {
  int i;
  for (i=0; i<KEYWORD_LENGTH; i++) {
	if (line[i] != keyword[i]) {
	  return FAIL;
	}
  }
  return SUCCESS;
}

void handle_n_of_definitions(char *line, int *n_of_definitions,
							 int **n_of_hydrogens,
							 char ****vertex_definitions) {
  if (sscanf(line+KEYWORD_LENGTH, " %d", n_of_definitions) != 1) {
	return;
  }
  if (*n_of_hydrogens == NULL) {
	*n_of_hydrogens = malloc(*n_of_definitions * sizeof(int));
  }
  if (*vertex_definitions == NULL) {
	*vertex_definitions = malloc(*n_of_definitions * sizeof(char **));
  }
}

void handle_angle(char *line, GRAPH *g) {
  double angle;
  if (sscanf(line+KEYWORD_LENGTH, " %lf", &angle) != 1) {
	return;
  }
  set_angle(g, angle);
}

void handle_distance(char *line, GRAPH *g) {
  double distance;
  if (sscanf(line+KEYWORD_LENGTH, " %lf", &distance) != 1) {
	return;
  }
  set_distance(g, distance);
}

void handle_domain_decomposition(char *line, GRAPH *g) {
  /* assumes that DIMENSIONS equals 3 */
  int decomposition[DIMENSIONS];
  if (sscanf(line+KEYWORD_LENGTH, " %d %d %d", decomposition,
			 (decomposition+1), (decomposition+2)) != DIMENSIONS) {
	return;
  }
  prepare_box(g, decomposition, NULL); /* NULL can be used with empty graph */
}

void handle_n_of_hydrogens(char *line, int **n_of_hydrogens,
						   char ****vertex_definitions, int index) {
  int number;
  if (sscanf(line+KEYWORD_LENGTH, " %d", &number) != 1) {
	return;
  }
  (*n_of_hydrogens)[index] = number;
  
  /* size for hydrogens, non-hydrogen and the residue-definition */
  (*vertex_definitions)[index] = malloc((number + 2) * sizeof(char *));
}

void malloc_and_copy(char *line, char ****vertex_definitions, int index,
					 int index2, int malloc_size) {
  int i;
  line += KEYWORD_LENGTH;
  (*vertex_definitions)[index][index2] = malloc(malloc_size * sizeof(char));
  while (*line == ' ') {
	line++; /* empty space removed */
  }
  for (i=0; i<malloc_size; i++) {
	(*vertex_definitions)[index][index2][i] = *(line+i);
	if (*(line+i) == 0 || *(line+i) == '\n' || *(line+i) == '\r') {
	  (*vertex_definitions)[index][index2][i] = 0;
	  break;
	}
  }
  (*vertex_definitions)[index][index2][malloc_size - 1] = 0;
}

void handle_residue_definition(char *line, char ****vertex_definitions,
							   int index) {
  malloc_and_copy(line, vertex_definitions, index, 0,
				  RESIDUE_DEFINITION_LENGTH);
}

void handle_atomname_definition(char *line, char ****vertex_definitions,
								int index, int index2) {
  malloc_and_copy(line, vertex_definitions, index, index2,
				  ATOMNAME_DEFINITION_LENGTH);
}

int build_vertex_definitions_and_prepare_graph(char *param, GRAPH *g,
											   char ****vertex_definitions,
											   int **n_of_hydrogens,
											   int *n_of_definitions) {
  FILE *param_pointer;
  char line[LINE_LENGTH_BUFFER];
  int index=-1, index2;

  param_pointer = fopen(param, "r");
  if (param_pointer == NULL) {
	printf("error opening file \"%s\"\n", param);
	return FAIL;
  }

  while (!feof(param_pointer)) {
	if (fgets(line, LINE_LENGTH_BUFFER, param_pointer) == NULL) {
	  break;
	}
	if (compare_keyword_and_line(line, N_OF_DEFINITIONS) == SUCCESS) {
	  handle_n_of_definitions(line, n_of_definitions, n_of_hydrogens,
							  vertex_definitions);
	} else if (compare_keyword_and_line(line, ANGLE) == SUCCESS) {
	  handle_angle(line, g);
	} else if (compare_keyword_and_line(line, DISTANCE) == SUCCESS) {
	  handle_distance(line, g);
	} else if (compare_keyword_and_line(line, DOMAIN_DECOMPOSITION)
			   == SUCCESS) {
	  handle_domain_decomposition(line, g);
	} else if (compare_keyword_and_line(line, N_OF_HYDROGENS) == SUCCESS) {
	  index++;
	  if (index >= *n_of_definitions) {
		printf("index %d >= n_of_definitions %d\n", index, *n_of_definitions);
		return FAIL;
	  }
	  handle_n_of_hydrogens(line, n_of_hydrogens, vertex_definitions, index);
	} else if (compare_keyword_and_line(line, RESIDUE_DEFINITION) == SUCCESS) {
	  if (index >= *n_of_definitions) {
		printf("index %d >= n_of_definitions %d\n", index, *n_of_definitions);
		return FAIL;
	  }
	  handle_residue_definition(line, vertex_definitions, index);
	  index2 = 1;
	} else if (compare_keyword_and_line(line, ATOMNAME_DEFINITION) == SUCCESS) {
	  if (index >= *n_of_definitions || index2 >= (*n_of_hydrogens)[index] + 2) {
		printf("index %d >=? n_of_definitions %d or index2 %d >= (*n_of_hydrogens)[%d] %d + 2\n", index, *n_of_definitions, index2, index, (*n_of_hydrogens)[index]);
		return FAIL;
	  }
	  handle_atomname_definition(line, vertex_definitions, index, index2);
	  index2++;
	}
  }

  return SUCCESS;
}

void free_vert_def_n_hydr(char ***vertex_definitions, int *n_of_hydrogens,
						  int n_def) {
  int i;
  for (i=0; i<n_def; i++) {
	int j;
	for (j=0; j<n_of_hydrogens[i]+2; j++) {
	  free(vertex_definitions[i][j]);
	}
	free(vertex_definitions[i]);
  }
  free(vertex_definitions);
  free(n_of_hydrogens);
}
