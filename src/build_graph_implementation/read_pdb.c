#include "build_graph.h"
#include "build_graph_implementation.h"
#include "constants.h"
#include "graph.h"
#include "graph_implementation.h"
#include <stdio.h>
#include <stdlib.h>

#define RESIDUE_LENGTH 3
#define RESID_LENGTH 4
#define SEGNAME_LENGTH 4
#define ATOMNAME_LENGTH 4

/* ncc means "number of common chars" */
int compare_substring(char * line, int start_index, int ncc, char * compare) {
  int i1=0, i2=start_index, i1_handled=0, i2_handled=0, common=0;
  if (compare[0] == 0) {
	return 0;
  }
  while (compare[i1] == ' ') {
	i1++;
	i1_handled++;
  }
  while (line[i2] == ' ') {
	i2++;
	i2_handled++;
  }
  
  while (i2_handled < ncc) {
	char c1,c2;
	if (compare[i1] == 0) {
	  c1=' ';
	} else {
	  c1=compare[i1];
	  i1++;
	}
	if (line[i2] == 0) {
	  c2=' ';
	} else {
	  c2=line[i2];
	  i2++;
	}
	if (c1 != c2) {
	  return -1;
	}
	if (c1 != ' ') {
	  common++;
	}
    i2_handled++;
    i1_handled++;
  }
  
  if (i2_handled == ncc) {
	return common;
  }
  return -1;
}

/* returns the number before definition, or a negative value if not matching */
int compare_definition_and_line(char * line, char * definition) {
  int number, i, ret;
  char residue[RESIDUE_LENGTH + 1];
  char segname[SEGNAME_LENGTH + 1];
  char residue_index[RESID_LENGTH + 1];
  ret = sscanf(definition, "%d %s %s %s", &number, residue, segname, residue_index);
  if (ret < 4) {
	residue_index[0] = 0;
	if (ret < 3) {
	  segname[0] = 0;
	}
  }
  for (i=1; i<RESID_LENGTH-1; i++) { /* skip the first in case of empty string */
	if (residue_index[i] == 0) {
	  residue_index[i] = ' ';
	  /* 
		 without this "21" would equal with "210" 
		 but now "21 " doesn't equal with "210"
	  */
	}
  }
  residue_index[RESID_LENGTH] = 0;
  
  if (compare_substring(line, 17, RESIDUE_LENGTH, residue) < 0) {
	return -1;
  } else if (compare_substring(line, 72, SEGNAME_LENGTH, segname) < 0) {
	return -1;
  } else if (compare_substring(line, 22, RESID_LENGTH, residue_index) < 0) {
	return -1;
  }
  return number;
}

int compare_atomname_and_line(char * line, char * atomname) {
  int hydrogen_or_not,i=1;
  char an[ATOMNAME_LENGTH+1] = "    ";
  while (atomname[i] != 0) {
	an[i-1] = atomname[i];
	i++;
  }
  if (compare_substring(line, 12, ATOMNAME_LENGTH, an) >= 0) {
	return (int) atomname[0] - '0'; /* this converts the first character to integer */
  }
  return 0;
}

void insert_atom(ATOM *a, char *line, int index) {
  int i;
  char *temp = line + 30; /* x-coordinate starts from here */
  double coord;
  a->index = index;
  sscanf(temp, " %8lf", &(a->coord));
  temp = line + 38; /* y-coordinate */
  sscanf(temp, " %8lf", &(a->coord[1]));
  temp = line + 46;
  sscanf(temp, " %8lf", &(a->coord[2]));
}

int is_an_atom(char *line, int *index) {
  if (compare_substring(line, 0, 6, "ATOM  ") >= 0) {
	(*index)++;
	/* 
	   program assumes that there is as many lines that 
	   start with ATOM as there are atoms in the system
	*/
	return SUCCESS;
  }
  return FAIL;
}

void compare_definition_and_atomname(char ***vertex_definitions, int *hydrogen_or_not,
									 int *index1, int *index2, char *line, int n_of_definitions, int *n_of_hydrogens) {
  int definition_number,i,j;
  for (i=0; i<n_of_definitions; i++) {	
	if ((definition_number = compare_definition_and_line(line, vertex_definitions[i][0])) >= 0) {
	  for (j=1; j <= n_of_hydrogens[i] + 1; j++) {
		if ((*hydrogen_or_not = compare_atomname_and_line(line, vertex_definitions[i][j])) > 0) {
		  *index1 = i;
		  *index2 = j;
		  return;
		}
	  }
	}
	if (definition_number == 0) {
	  break;
	}
  } 
}

int read_pdb(char *pdb, char ***vertex_definitions, int *n_of_hydrogens, int n_of_definitions, GRAPH *g) {
  FILE *pdb_pointer;
  char line[PDB_LINE_LENGTH_BUFFER];
  int hydrogen_index=-2, index=-1, number_of_vertexes=0, g_index=0;

  pdb_pointer = fopen(pdb, "r");
  if (pdb_pointer == NULL) {
	printf("error opening file \"%s\"\n", pdb);
	return FAIL;
  }

  /*
	In order to avoid allocating memory for each vertex outside graph and then again inside
	it is better to first calculate the amount of vertex needed and only allocate the 
	memory inside the graph (otherwise there will probably be a segmentation fault).
   */
  while (!feof(pdb_pointer)) { 
	int i=-1,j,hydrogen_or_not;
	fgets(line, PDB_LINE_LENGTH_BUFFER, pdb_pointer);
	if (is_an_atom(line, &index) == FAIL) {
	  continue;
	}
	compare_definition_and_atomname(vertex_definitions, &hydrogen_or_not, &i, &j, line, n_of_definitions, n_of_hydrogens);
	if (i == -1) {
	  continue;
	}
	if (j == 1 && hydrogen_index < 0) {
	  hydrogen_index=0;
	}
	if (hydrogen_or_not == HYDROGEN_IDENTIFIER && hydrogen_index < n_of_hydrogens[i]) {
	  hydrogen_index++;
	}
	if (hydrogen_index == n_of_hydrogens[i]) {
	  number_of_vertexes++;
	  hydrogen_index=-2;
	}
  }
  set_vertex_list_length(g, number_of_vertexes); /* here the allocation is done with one step */
  g->number_of_nodes = number_of_vertexes;

  rewind(pdb_pointer); /* in order to read the file again */
  hydrogen_index=-2; /* this is also important */
  index=-1; /* and this */

  while (!feof(pdb_pointer)) {
	int i=-1,j,hydrogen_or_not;
	VERTEX *v; /* this gets initialized if something is found */
	fgets(line, PDB_LINE_LENGTH_BUFFER, pdb_pointer);
	if (is_an_atom(line, &index) == FAIL) {
	  continue;
	}
	compare_definition_and_atomname(vertex_definitions, &hydrogen_or_not, &i, &j, line, n_of_definitions, n_of_hydrogens);
	if (i == -1) {
	  continue;
	}
	if (j == 1 && hydrogen_index < 0) {
	  v = &(g->nodes[g_index]);/*new_vertex();*/ /* found the first atom of vertex */
	  g_index++;
	  initialize_hydrogens(v, n_of_hydrogens[i]);
	  hydrogen_index=0;
	}
	if (hydrogen_or_not == HYDROGEN_IDENTIFIER && hydrogen_index < n_of_hydrogens[i]) {
	  insert_atom(&(v->hydrogens[hydrogen_index]), line, index);
	  hydrogen_index++;
	}
	if (hydrogen_or_not == NON_HYDROGEN_IDENTIFIER) {
	  insert_atom(&(v->not_hydrogen), line, index);
	}
	if (hydrogen_index == n_of_hydrogens[i]) {
	  /*add_vertex(g, v);*/ /* this should be the last */
	  hydrogen_index = -2;
	  if (g_index >= g->number_of_nodes) {
		break;
	  }
	}
  }
  
  fclose(pdb_pointer);
  return SUCCESS;
}
