#include "build_graph.h"
#include "build_graph_implementation.h"
#include "constants.h"
#include "graph.h"
#include "graph_implementation.h"
#include <stdio.h>
#include <stdlib.h>

#include "xtcio.h" /* this needs GROMACS */

/* the global variables of read_xtc */
t_fileio  *xd = NULL;
real t = 0, prec = 0;
matrix box;
rvec *x = NULL;
gmx_bool bOK = 0;
int natoms,step;

void set_atom_coordinates(ATOM *a) {
  int i;
  for (i=0; i<DIMENSIONS; i++) {
	a->coord[i] = x[a->index][i];
  }
}

void set_all_atom_coordinates(GRAPH *g) {
  int i,j;
  COORDINATE vector[DIMENSIONS];
  int decomp[DIMENSIONS];
  for (i=0; i < g->number_of_nodes; i++) {
	VERTEX *v = &(g->nodes[i]);
	set_atom_coordinates(&(v->not_hydrogen));
	for (j=0; j < v->n_hydrogens; j++) {
	  set_atom_coordinates(&(v->hydrogens[j]));
	}
  }
  for (i=0; i<DIMENSIONS; i++) {
	decomp[i] = g->box->decomposition[i];
	for (j=0; j<DIMENSIONS; j++) {
	  vector[i][j] = box[i][j];
	}
  }
  /* uses the same decomposition as before */
  prepare_box(g, decomp, vector);
  /*for (i=0; i<DIMENSIONS; i++) {
	for (j=0; j<DIMENSIONS; j++) {
	  if (vector[i][j] != box[i][j]) {
		printf("failed, g->box[%d][%d] != box[%d][%d] but instead: %f != %f\n",
			   i, j, i, j, g->box->vectors[i][j], box[i][j]);
	  }
	}
	}*/
}

int read_first_xtc_pathfinder(char *xtc, GRAPH *g) {
  if ((xd = open_xtc(xtc, "r")) == 0) {
	printf("opening xtc file %s failed\n", xtc);
	return FAIL;
  }
  if (read_first_xtc(xd, &natoms, &step, &t, box, &x, &prec, &bOK) == 0) {
	printf("reading first frame failed\n");
	return FAIL;
  }
  set_all_atom_coordinates(g);
  return SUCCESS;
}

int read_next_xtc_pathfinder(GRAPH *g) {
  if (read_next_xtc(xd, natoms, &step, &t, box, x, &prec, &bOK) == 0) {
	printf("reading frame %d failed\n", step);
	return FAIL;
  }
  set_all_atom_coordinates(g);
  return SUCCESS;
}

int close_xtc_pathfinder() {
  close_xtc(xd);
  xd = NULL;
  free(x);
  x = NULL;
  return SUCCESS;
}
