#include "unity.h" /* I have downloaded unity from http://www.throwtheswitch.org/unity/ */
#include "a_star.h"
#include "heap.h"
#include "constants.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void test_new_a_star(void) {
  A_STAR *a = new_a_star();
  TEST_ASSERT_EQUAL_INT(-1, a->start);
  TEST_ASSERT_EQUAL_INT(-1, a->target);
  TEST_ASSERT_EQUAL_INT(0, a->n_of_nodes);
  TEST_ASSERT(NULL == a->distance_from_start);
  TEST_ASSERT(NULL == a->came_from);
  delete_a_star(a);
}

void test_set_start_target(void) {
  A_STAR *a = new_a_star();
  set_start(a, 3);
  set_target(a, 5);
  TEST_ASSERT_EQUAL_INT(3, a->start);
  TEST_ASSERT_EQUAL_INT(5, a->target);
  delete_a_star(a);
}

void test_path_finding(void) {
  
}

int main(void) {
	printf("\n");
	UNITY_BEGIN();
	RUN_TEST(test_new_a_star);
	RUN_TEST(test_set_start_target);
	return UNITY_END();
}
