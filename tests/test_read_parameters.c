#include "unity.h" /* I have downloaded unity from http://www.throwtheswitch.org/unity/ */
#include "graph.h"
#include "graph_implementation.h"
#include "constants.h"
#include "build_graph.h"
#include "build_graph_implementation.h"
#include <stdlib.h>
#include <stdio.h>

void test_compare_keyword_and_line(void) {
  TEST_ASSERT_EQUAL_INT(SUCCESS,
						compare_keyword_and_line("NDEFjou jou", "NDEF"));
  TEST_ASSERT_EQUAL_INT(FAIL,
						compare_keyword_and_line("NDEFjou jou", "ATOM"));
}

void test_handle_n_of_definitions(void) {
  int n_def = -1;
  int *n_hydr = NULL; /* remember to put these first NULL */
  char ***vert_def = NULL;
  handle_n_of_definitions("random", &n_def, &n_hydr, &vert_def);
  TEST_ASSERT_EQUAL_INT(-1, n_def);
  handle_n_of_definitions("NDEF 3", &n_def, &n_hydr, &vert_def);
  TEST_ASSERT_EQUAL_INT(3, n_def);
  free(n_hydr);
  free(vert_def);
}

void test_handle_angle(void) {
  GRAPH *g = new_graph();
  handle_angle("ANGL 35.5", g);
  /* the function doesn't care about the keyword, only it's length */
  TEST_ASSERT_EQUAL_FLOAT(35.5, g->angle);
  handle_angle("ang34.5", g);
  TEST_ASSERT_EQUAL_FLOAT(4.5, g->angle);
  delete_graph(&g);
}

void test_handle_distance(void) {
  GRAPH *g = new_graph();
  handle_distance("DIST 151.6", g);
  TEST_ASSERT_EQUAL_FLOAT(151.6, g->distance);
  delete_graph(&g);
}

void test_domain_decomposition(void) {
  GRAPH *g = new_graph();
  handle_domain_decomposition("---- 4 5 6", g);
  TEST_ASSERT_EQUAL_INT(4, g->box->decomposition[0]);
  TEST_ASSERT_EQUAL_INT(5, g->box->decomposition[1]);
  TEST_ASSERT_EQUAL_INT(6, g->box->decomposition[2]);
  delete_graph(&g);
}

void test_n_of_hydrogens(void) {
  int n_def = -1;
  int *n_hydr = NULL; /* remember to put these first NULL */
  char ***vert_def = NULL;
  handle_n_of_definitions("---- 1", &n_def, &n_hydr, &vert_def);
  handle_n_of_hydrogens("---- 2", &n_hydr, &vert_def, 0);
  TEST_ASSERT_EQUAL_INT(2, n_hydr[0]);
  free(n_hydr);
  free(vert_def[0]);
  free(vert_def);
  /*free_vert_def_n_hydr(vert_def, n_hydr, n_def);*/
}

void test_handle_residue_definition_and_atomname(void) {
  int n_def = -1;
  int *n_hydr = NULL; /* remember to put these first NULL */
  char ***vert_def = NULL;
  handle_n_of_definitions("---- 1", &n_def, &n_hydr, &vert_def);
  handle_n_of_hydrogens("---- 2", &n_hydr, &vert_def, 0);
  handle_residue_definition("---- 0 ARG PROA", &vert_def, 0);
  TEST_ASSERT_EQUAL_STRING("0 ARG PROA", vert_def[0][0]);
  handle_atomname_definition("---- 5 CA ", &vert_def, 0, 2);
  TEST_ASSERT_EQUAL_STRING("5 CA ", vert_def[0][2]);
  free(n_hydr);
  free(vert_def[0][0]);
  free(vert_def[0][2]);
  free(vert_def[0]);
  free(vert_def);  
}

void test_reading_parameter_file(void) {
  int n_def = -1;
  int *n_hydr = NULL; /* remember to put these first NULL */
  char ***vert_def = NULL;
  GRAPH *g = new_graph();
  

  build_vertex_definitions_and_prepare_graph("parameter_file_test.txt",
											 g, &vert_def, &n_hydr, &n_def);

  TEST_ASSERT_EQUAL_FLOAT(45.0, g->angle);
  TEST_ASSERT_EQUAL_FLOAT(17.0, g->distance);
  TEST_ASSERT_EQUAL_INT(2, g->box->decomposition[0]);
  TEST_ASSERT_EQUAL_INT(4, g->box->decomposition[1]);
  TEST_ASSERT_EQUAL_INT(6, g->box->decomposition[2]);
  TEST_ASSERT_EQUAL_INT(2, n_def);
  TEST_ASSERT_EQUAL_INT(2, n_hydr[0]);
  TEST_ASSERT_EQUAL_INT(1, n_hydr[1]);
  TEST_ASSERT_EQUAL_STRING("0 ARG PROA", vert_def[0][0]);
  TEST_ASSERT_EQUAL_STRING("1 GB", vert_def[0][1]);
  TEST_ASSERT_EQUAL_STRING("2 H3", vert_def[0][2]);
  TEST_ASSERT_EQUAL_STRING("2 H5", vert_def[0][3]);
  TEST_ASSERT_EQUAL_STRING("0 JEA", vert_def[1][0]);
  TEST_ASSERT_EQUAL_STRING("1 hub", vert_def[1][1]);
  TEST_ASSERT_EQUAL_STRING("2jeba", vert_def[1][2]);	
  
  free_vert_def_n_hydr(vert_def, n_hydr, n_def);
  delete_graph(&g);
}

int main(void) {
  printf("\n");
  UNITY_BEGIN();
  RUN_TEST(test_compare_keyword_and_line);
  RUN_TEST(test_handle_n_of_definitions);
  RUN_TEST(test_handle_angle);
  RUN_TEST(test_handle_distance);
  RUN_TEST(test_domain_decomposition);
  RUN_TEST(test_n_of_hydrogens);
  RUN_TEST(test_handle_residue_definition_and_atomname);
  RUN_TEST(test_reading_parameter_file);
  return UNITY_END();
}
