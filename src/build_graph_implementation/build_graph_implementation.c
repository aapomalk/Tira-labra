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

int read_pdb(char *pdb, char ***vertex_definitions, int *n_of_hydrogens, int n_of_definitions, GRAPH *g) {
  FILE *pdb_pointer;
  char line[PDB_LINE_LENGTH_BUFFER];
  int hydrogen_index=-2, index=-1;

  pdb_pointer = fopen(pdb, "r");
  if (pdb_pointer == NULL) {
	printf("error opening file \"%s\"\n", pdb);
	return FAIL;
  }
  
  while (!feof(pdb_pointer)) {
	int i,j;
	VERTEX *v; /* this gets initialized if something is found */
	fgets(line, PDB_LINE_LENGTH_BUFFER, pdb_pointer);
	if (is_an_atom(line, &index) == FAIL) {
	  continue;
	}
	for (i=0; i<n_of_definitions; i++) {
	  int definition_number;
	  
	  if ((definition_number = compare_definition_and_line(line, vertex_definitions[i][0])) >= 0) {
		for (j=1; j <= n_of_hydrogens[i] + 1; j++) {
		  int hydrogen_or_not;
		  if ((hydrogen_or_not = compare_atomname_and_line(line, vertex_definitions[i][j])) > 0) {
			if (j == 1 && hydrogen_index < 0) {
			  printf("new ");
			  v = new_vertex(); /* found the first atom of vertex */
			  printf("a ");
			  initialize_hydrogens(v, n_of_hydrogens[i]);
			  printf("vertex ");
			  hydrogen_index=0;
			}
			if (hydrogen_or_not == HYDROGEN_IDENTIFIER && hydrogen_index < n_of_hydrogens[i]) {
			  printf("inserting ");
			  insert_atom(&(v->hydrogens[hydrogen_index]), line, index);
			  printf("jea ");
			  hydrogen_index++;
			}
			if (hydrogen_or_not == NON_HYDROGEN_IDENTIFIER) {
			  printf("not ");
			  insert_atom(&(v->not_hydrogen), line, index);
			  printf("h ");
			}
			if (hydrogen_index == n_of_hydrogens[i]) {
			  printf("added vertex %d  ", index);
			  add_vertex(g, v); /* this should be the last */
			  printf("end\n");
			  hydrogen_index = -2;
			}
		  }
		}
	  }
	  if (definition_number == 0) {
		break;
	  }
	}
  }
  
  fclose(pdb_pointer);
  return SUCCESS;
}
