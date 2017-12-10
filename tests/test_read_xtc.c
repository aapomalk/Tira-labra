#include "unity.h" /* I have downloaded unity from http://www.throwtheswitch.org/unity/ */
#include "graph.h"
#include "graph_implementation.h"
#include "constants.h"
#include "build_graph.h"
#include "build_graph_implementation.h"
#include "a_star.h"
#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

void test_read_xtc(void) {
  int n_def = -1;
  int *n_hydr = NULL; /* remember to put these first NULL */
  char ***vert_def = NULL;
  GRAPH *g = new_graph();
  char *pdb = "../xtc_reader/final_solv_ions.pdb";
  char *xtc = "/home/work/aapomalk/proteins/complex_iv_monomer/separated/final_no_hetero2/sisu_trajectories/md.part0005_clst_cntr.xtc";
  /* replace xtc and the pdb with some compatible files */

  double test_coordinate = 0;
  int i;

  A_STAR *a = new_a_star();
  int found;
  set_start(a, 0);
  set_target(a, 1);
  set_limiter(a, 1.5);
  
  if (build_vertex_definitions_and_prepare_graph("parameter_protein_and_water.txt",
												 g, &vert_def, &n_hydr, &n_def) == FAIL) {
	printf("parameters failed\n");
	return;
  }

  read_pdb(pdb, vert_def, n_hydr, n_def, g);

  test_coordinate = g->nodes[0].not_hydrogen.coord[0];
  
  TEST_ASSERT_EQUAL_INT(SUCCESS, read_first_xtc_pathfinder(xtc, g));

  TEST_ASSERT(test_coordinate != g->nodes[0].not_hydrogen.coord[0]);
  test_coordinate = g->nodes[0].not_hydrogen.coord[0];

  found = search_path(a, g);
  printf("path found %d, length %f\n", found, get_path_length(a));
  
  for (i=0; i>=0; i++) {
	if (SUCCESS != read_next_xtc_pathfinder(g)) {
	  printf("exiting loop at round %d\n", i);
	  break;
	}
	/*TEST_ASSERT(test_coordinate != g->nodes[0].not_hydrogen.coord[0]);*/
	found = search_path(a, g);
	printf("step %d, path found %d, length %f\n", i, found, get_path_length(a));
  }

  TEST_ASSERT_EQUAL_INT(SUCCESS, close_xtc_pathfinder());
  free_vert_def_n_hydr(vert_def, n_hydr, n_def);
  delete_graph(&g);
}

int main(void) {
  printf("\n");
  UNITY_BEGIN();
  RUN_TEST(test_read_xtc);
  return UNITY_END();
}
