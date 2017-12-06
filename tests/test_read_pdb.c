#include "unity.h" /* I have downloaded unity from http://www.throwtheswitch.org/unity/ */
#include "graph.h"
#include "graph_implementation.h"
#include "constants.h"
#include "build_graph.h"
#include "build_graph_implementation.h"
#include <stdlib.h>
#include <stdio.h>

#define INITIALIZE_LINE char line[] = "ATOM     14  CE  MET P   1      61.3391164.952  51.391  1.00  0.01      PROA C"

void test_compare_substring(void) {
  TEST_ASSERT_EQUAL_INT_MESSAGE(-1, compare_substring("jou",0,3,"jea"), "first");
  TEST_ASSERT_EQUAL_INT_MESSAGE(3, compare_substring("jou",0,3,"jou"), "first2");
  TEST_ASSERT_EQUAL_INT_MESSAGE(3, compare_substring("joujeajou",3,3,"jea"), "second");
  TEST_ASSERT_EQUAL_INT_MESSAGE(3, compare_substring("jou jeajou",3,4," jea"), "second2");
  TEST_ASSERT_EQUAL_INT_MESSAGE(3, compare_substring("jou jeajou",3,4,"jea"), "second22");
  TEST_ASSERT_EQUAL_INT_MESSAGE(3, compare_substring("joujea jou",3,4,"jea"), "second3");
  TEST_ASSERT_EQUAL_INT_MESSAGE(3, compare_substring("joujea jou",3,4," jea"), "second32");
  TEST_ASSERT_EQUAL_INT_MESSAGE(3, compare_substring("   jeajou",0,6,"jea"), "third");
  TEST_ASSERT_EQUAL_INT_MESSAGE(3, compare_substring("   jeajou",0,6,"   jea"), "fourth");
}

void test_compare_definition_and_line(void) {
  INITIALIZE_LINE;

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, compare_definition_and_line(line, "0 MET"), "0 MET");
  TEST_ASSERT_EQUAL_INT_MESSAGE(-1, compare_definition_and_line(line, "0 PRO"), "0 PRO");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, compare_definition_and_line(line, "0 MET PROA"), "0 MET PROA");
  TEST_ASSERT_EQUAL_INT_MESSAGE(2, compare_definition_and_line(line, "2 MET PROA"), "0 MET PROA 2");
  TEST_ASSERT_EQUAL_INT_MESSAGE(-1, compare_definition_and_line(line, "0 MET PROB"), "0 MET PROB");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, compare_definition_and_line(line, "0 MET PROA 1"), "0 MET PROA 1");
  TEST_ASSERT_EQUAL_INT_MESSAGE(-1, compare_definition_and_line(line, "0 MET PROA 3"), "0 MET PROA 3");

  TEST_ASSERT_EQUAL_INT(5, compare_definition_and_line(line, "5 MET PROA 1"));
}

void test_compare_atomname_and_line(void) {
  INITIALIZE_LINE;

  TEST_ASSERT_EQUAL_INT(1, compare_atomname_and_line(line, "1CE "));
  TEST_ASSERT_EQUAL_INT(1, compare_atomname_and_line(line, "1CE"));
  TEST_ASSERT_EQUAL_INT(0, compare_atomname_and_line(line, "1C"));
  TEST_ASSERT_EQUAL_INT(0, compare_atomname_and_line(line, "1C "));
  TEST_ASSERT_EQUAL_INT(3, compare_atomname_and_line(line, "3CE "));
  TEST_ASSERT_EQUAL_INT(1, compare_atomname_and_line(line, "1 CE "));
  TEST_ASSERT_EQUAL_INT(0, compare_atomname_and_line(line, "3ABC "));
}

void test_insert_atom(void) {
  INITIALIZE_LINE;
  ATOM atom;
  insert_atom(&atom, line, 3);
  TEST_ASSERT_EQUAL_INT(3, atom.index);
  TEST_ASSERT_EQUAL_FLOAT(61.339, atom.coord[0]);
  TEST_ASSERT_EQUAL_FLOAT(1164.952, atom.coord[1]); /* works even if no space between numbers */
  TEST_ASSERT_EQUAL_FLOAT(51.391, atom.coord[2]);
}

void test_is_an_atom(void) {
  INITIALIZE_LINE;
  int index = 3;

  TEST_ASSERT_EQUAL_INT(3, index);
  TEST_ASSERT_EQUAL_INT(SUCCESS, is_an_atom(line, &index)); /* the index grows with one */
  TEST_ASSERT_EQUAL_INT(4, index);
  TEST_ASSERT_EQUAL_INT(FAIL, is_an_atom("WRONG LINE JOU JOU 1 2 3 ", &index)); /* wrong line keeps the index same */
  TEST_ASSERT_EQUAL_INT(4, index);
}

void test_read_pdb(void) {
  char *pdb = "../xtc_reader/final_solv_ions.pdb";
  int def = 3;
  char ***vertex_definitions = malloc(def*sizeof(char**));
  int *n_of_hydrogens = malloc(def*sizeof(int));
  GRAPH *g = new_graph();
  int i,j,previous=-1;

  vertex_definitions[0] = malloc(4*sizeof(char*)); /* room for two hydrogens */
  vertex_definitions[0][0] = "1 LEU PROA 21";
  vertex_definitions[0][1] = "1CB";
  vertex_definitions[0][2] = "2HB1";
  vertex_definitions[0][3] = "2HB2";
  n_of_hydrogens[0] = 2;

  vertex_definitions[1] = malloc(3*sizeof(char*)); /* room for one hydrogen */
  vertex_definitions[1][0] = "0 LEU PROA 21";
  vertex_definitions[1][1] = "1CG";
  vertex_definitions[1][2] = "2HG";
  n_of_hydrogens[1] = 1;

  vertex_definitions[2] = malloc(4*sizeof(char*)); /* room for one hydrogen */
  vertex_definitions[2][0] = "0 TIP3";
  vertex_definitions[2][1] = "1OH2";
  vertex_definitions[2][2] = "2H1";
  vertex_definitions[2][3] = "2H2";
  n_of_hydrogens[2] = 2;


  TEST_ASSERT_EQUAL_INT(SUCCESS, read_pdb(pdb, vertex_definitions, n_of_hydrogens, def, g));
  TEST_ASSERT_EQUAL_INT(64698, g->number_of_nodes);
  
  /* see the indexes from pdb file in xtc_reader folder */
  TEST_ASSERT_EQUAL_INT(352, g->nodes[0].not_hydrogen.index);
  /* note the difference in pdb file (353) and in this program */

  TEST_ASSERT_EQUAL_INT(353, g->nodes[0].hydrogens[0].index);
  TEST_ASSERT_EQUAL_INT(354, g->nodes[0].hydrogens[1].index);

  for (i=0; i<g->number_of_nodes; i++) {
	/*printf("index %d %d %d %.3f %.3f %.3f\n", i, g->nodes[i].not_hydrogen.index, previous,
	  g->nodes[i].not_hydrogen.coord[0], g->nodes[i].not_hydrogen.coord[1], g->nodes[i].not_hydrogen.coord[2]);*/
	TEST_ASSERT(previous < g->nodes[i].not_hydrogen.index);
	previous = g->nodes[i].not_hydrogen.index;
	for (j=0; j<g->nodes[i].n_hydrogens; j++) {
	  /*printf("hydrogen %d %d %d %.3f %.3f %.3f\n", j, g->nodes[i].hydrogens[j].index, previous,
		g->nodes[i].hydrogens[j].coord[0], g->nodes[i].hydrogens[j].coord[1], g->nodes[i].hydrogens[j].coord[2]);*/
	  TEST_ASSERT(previous < g->nodes[i].hydrogens[j].index);
	  previous = g->nodes[i].hydrogens[j].index;
	}
  }
  
  free(n_of_hydrogens);
  free(vertex_definitions[0]);
  free(vertex_definitions);
}

int main(void) {
  printf("\n");
  UNITY_BEGIN();
  RUN_TEST(test_compare_substring);
  RUN_TEST(test_compare_definition_and_line);
  RUN_TEST(test_compare_atomname_and_line);
  RUN_TEST(test_insert_atom);
  RUN_TEST(test_is_an_atom);
  RUN_TEST(test_read_pdb);
  return UNITY_END();
}

