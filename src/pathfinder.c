#include "graph.h"
#include "graph_implementation.h"
#include "constants.h"
#include "build_graph.h"
#include "build_graph_implementation.h"
#include "a_star.h"
#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

#define TOPOLOGY "-pdb"
#define PARAMETER "-par"
#define XTC "-xtc"
#define START "-start"
#define END "-end"
#define LIMITER "-limiter"

int string_equal(char * string1, char * string2) {
  while (*string1 != 0 && *string2 != 0) {
	if (*string1 != *string2) {
	  return FAIL;
	}
	string1++;
	string2++;
  }
  if (*string1 != *string2) {
	return FAIL;
  }
  return SUCCESS;
}

void usage() {
  printf("program needs three files: pdb, xtc and parameter -file\n");
  printf("each file is given with two arguments like this:\n");
  printf("./program_name %s pdb_file_name.pdb %s xtc_file.xtc %s parameters.txt\n",
		 TOPOLOGY, XTC, PARAMETER);
  printf("\nadditionally the program needs at least starting index and ending index\n");
  printf("all the rest of the arguments are given as:\n");
  printf("%s starting_index\n", START);
  printf("%s ending_index\n", END);
  printf("%s limiter\n", LIMITER);
  printf("There can be several starts and ends and limiters.\n");
  printf("These three kinds of arguments are read from left to right and at least one starting_index needs to be before ending_index\n");
  printf("Pathfinder is started always at ending_index.\n");
}

int run_pathfinder(int argc, char ** argv) {
  char *pdb = NULL, *xtc = NULL, *param = NULL; /* filenames */
  int n_def = -1;
  int *n_hydr = NULL; /* remember to put these first NULL */
  char ***vert_def = NULL;
  GRAPH *g = new_graph();
  A_STAR *a = new_a_star();
  int i;
  double limiter=-1;
  int *failed_arguments = calloc(argc, sizeof(int));
  int did_something = 0;

  if (failed_arguments == NULL) {
	delete_graph(&g);
	delete_a_star(a);
	printf("calloc failed\n");
	return FAIL;
  }
  for (i=0; i<argc; i++) {
	printf("%s ", argv[i]); /* printing the command */
  }
  printf("\n");
  
  printf("pathfinder started\n");
  
  set_limiter(a, limiter); /* this can be replaced by arguments */
  
  for (i=1; i<argc-1; i++) { /* first just check the three files */
	if (SUCCESS == string_equal(argv[i], TOPOLOGY)) {
	  i++;
	  pdb = argv[i];
	  
	} else if (SUCCESS == string_equal(argv[i], PARAMETER)) {
	  i++;
	  param = argv[i];
	  
	} else if (SUCCESS == string_equal(argv[i], XTC)) {
	  i++;
	  xtc = argv[i];
	  
	}
  } /* end of checking filenames */

  if (pdb == NULL || xtc == NULL || param == NULL) {
	printf("missing files\n");
	usage();
	delete_graph(&g);
	delete_a_star(a);
	free(failed_arguments);
	return FAIL;
  }
  printf("reading parameters..\n");
  if (build_vertex_definitions_and_prepare_graph(param,
												 g, &vert_def,
												 &n_hydr,
												 &n_def) == FAIL) {
	printf("parameters failed\n");
	delete_graph(&g);
	delete_a_star(a);
	free(failed_arguments);
	return FAIL;
  } /* parameters */
  printf("using: angle %f, distance %f, domain decomposition %d %d %d\n",
		 g->angle, g->distance, g->box->decomposition[0],
		 g->box->decomposition[1], g->box->decomposition[2]);
  printf("number of vertex definitions: %d, see %s in file %s\n", n_def, (n_def < 2) ? "it" : "them", param);
  
  printf("reading pdb..\n");
  if (read_pdb(pdb, vert_def, n_hydr, n_def, g) == FAIL) {
	printf("pdb reading failed\n");
	delete_graph(&g);
	delete_a_star(a);
	free_vert_def_n_hydr(vert_def, n_hydr, n_def);
	free(failed_arguments);
	return FAIL;
  } /* pdb */
  
  printf("reading xtc..\n");
  if (read_first_xtc_pathfinder(xtc, g) == FAIL) {
	printf("xtc first frame failed\n");
	delete_graph(&g);
	delete_a_star(a);
	free_vert_def_n_hydr(vert_def, n_hydr, n_def);
	close_xtc_pathfinder();
	free(failed_arguments);
	return FAIL;
  } /* first frame of xtc */

  printf("starting path finding\n");
  printf("\nstep, length, list of structure_index\n");
  for (i=0; i>=0; i++) {
	int j,found;
	for (j=1; j<argc-1; j++) {
	  if (failed_arguments[j] == 1) {
		continue;
	  }
	  
	  if (SUCCESS == string_equal(argv[j], START)) {
		int structure_index;
		VERTEX *v;
		j++;
		if (sscanf(argv[j], "%d", &structure_index) != 1) {
		  printf("reading starting index failed %s\n", argv[j]);
		  failed_arguments[j-1] = 1;
		  continue;
		}
		v = find_vertex(g, structure_index);
		if (v == NULL) {
		  printf("vertex NULL %s\n", argv[j]);
		  failed_arguments[j-1] = 1;
		  continue;
		}
		set_start(a, v->index);
		
	  } else if (SUCCESS == string_equal(argv[j], LIMITER)) {
		j++;
		if (sscanf(argv[j], "%lf", &limiter) != 1) {
		  printf("reading limiter failed %s\n", argv[j]);
		  failed_arguments[j-1] = 1;
		  continue;
		}
		set_limiter(a, limiter);
		
	  } else if (SUCCESS == string_equal(argv[j], END)) {
		int structure_index;
		VERTEX *v;
		j++;
		if (sscanf(argv[j], "%d", &structure_index) != 1) {
		  printf("reading ending index failed %s\n", argv[j]);
		  failed_arguments[j-1] = 1;
		  continue;
		}
		v = find_vertex(g, structure_index);
		if (v == NULL) {
		  printf("vertex NULL %s\n", argv[j]);
		  failed_arguments[j-1] = 1;
		  continue;
		}
		set_target(a, v->index);

		found = search_path(a, g);
		did_something = 1;

		if (found == SUCCESS) {
		  int *indexes,k,steps = -1;
		  printf("%d, %f, ", i, get_path_length(a));
		  indexes = get_path_indexes(a, &steps);
		  for (k=steps-1; k>=0; k--) {
			printf("%d ", g->nodes[indexes[k]].not_hydrogen.index);
		  }
		  printf("\n");
		  free(indexes);
		}
		
		
	  } /* if, else if */
	} /* for j */

	if (did_something == 0) {
	  printf("no working indexes etc\n");
	  break;
	}
	did_something = 0;

	if (SUCCESS != read_next_xtc_pathfinder(g)) {
	  printf("exiting loop at round %d\n", i);
	  break;
	} /* next frame of xtc */
  } /* for i */

  delete_graph(&g);
  delete_a_star(a);
  free_vert_def_n_hydr(vert_def, n_hydr, n_def);
  close_xtc_pathfinder();
  free(failed_arguments);
  return SUCCESS;
}
