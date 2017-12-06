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
							 int **n_of_hydrogens, char ****vertex_definitions) {
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

int build_vertex_definitions_and_prepare_graph(char *param, GRAPH *g,
											   char ****vertex_definitions,
											   int **n_of_hydrogens,
											   int *n_of_definitions) {
  FILE *param_pointer;
  char line[LINE_LENGTH_BUFFER];

  param_pointer = fopen(param, "r");
  if (param_pointer == NULL) {
	printf("error opening file \"%s\"\n", param);
	return FAIL;
  }

  while (!feof(param_pointer)) {
	int i;
	fgets(line, LINE_LENGTH_BUFFER, param_pointer);
	if (compare_keyword_and_line(line, N_OF_DEFINITIONS) == SUCCESS) {
	  handle_n_of_definitions(line, n_of_definitions, n_of_hydrogens,
							  vertex_definitions);
	} else if (compare_keyword_and_line(line, ANGLE) == SUCCESS) {

	} else if (compare_keyword_and_line(line, DISTANCE) == SUCCESS) {

	} else if (compare_keyword_and_line(line, DOMAIN_DECOMPOSITION) == SUCCESS) {

	} else if (compare_keyword_and_line(line, N_OF_HYDROGENS) == SUCCESS) {

	} else if (compare_keyword_and_line(line, RESIDUE_DEFINITION) == SUCCESS) {

	} else if (compare_keyword_and_line(line, ATOMNAME_DEFINITION) == SUCCESS) {

	}
  }

  return SUCCESS;
}
