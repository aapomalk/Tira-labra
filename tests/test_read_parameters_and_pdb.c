#include "unity.h" /* I have downloaded unity from http://www.throwtheswitch.org/unity/ */
#include "graph.h"
#include "graph_implementation.h"
#include "constants.h"
#include "build_graph.h"
#include "build_graph_implementation.h"
#include <stdlib.h>
#include <stdio.h>

void test_read_parameters_and_pdb(void) {
  int n_def = -1;
  int *n_hydr = NULL; /* remember to put these first NULL */
  char ***vert_def = NULL;
  GRAPH *g = new_graph();
  char *pdb = "../xtc_reader/final_solv_ions.pdb";

  build_vertex_definitions_and_prepare_graph("parameter_water.txt",
											 g, &vert_def, &n_hydr, &n_def);
  
  TEST_ASSERT_EQUAL_INT(SUCCESS,
						read_pdb(pdb, vert_def,
								 n_hydr, n_def, g));

  TEST_ASSERT_EQUAL_INT(64696, g->number_of_nodes);
  
  free_vert_def_n_hydr(vert_def, n_hydr, n_def);
  delete_graph(&g);
}

int main(void) {
  printf("\n");
  UNITY_BEGIN();
  RUN_TEST(test_read_parameters_and_pdb);
  return UNITY_END();
}
