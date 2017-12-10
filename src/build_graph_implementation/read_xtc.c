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
real time = 0, prec = 0;
matrix box = NULL;
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
  int i;
  for (i=0; i < g->number_of_nodes; i++) {
	VERTEX *v = &(g->nodes[i]);
	int j;
	set_atom_coordinates(&(v->not_hydrogen));
	for (j=0; j < v->n_hydrogens; j++) {
	  set_atom_coordinates(&(v->hydrogens[j]));
	}
  }
  /* uses the same decomposition as before */
  prepare_box(g, g->box->decomposition, box);
}

int read_first_xtc_pathfinder(char *xtc, GRAPH *g) {
  if ((xd = open_xtc(xtc, "r")) == 0) {
	printf("opening xtc file %s failed\n", xtc);
	return FAIL;
  }
  if (read_first_xtc(xd, &natoms, &step, &time, box, &x, &prec, &bOK) == 0) {
	printf("reading first frame failed\n");
	return FAIL;
  }
  set_all_atom_coordinates(g);
}

int read_next_xtc_pathfinder(GRAPH *g) {
  if (read_next_xtc(xd, &natoms, &step, &time, box, &x, &prec, &bOK) == 0) {
	printf("reading frame %d failed\n", step);
	return FAIL;
  }
  set_all_atom_coordinates(g);
}

int close_xtc_pathfinder() {
  close_xtc(xd);
  xd = NULL;
  free(x);
  x = NULL;
}
