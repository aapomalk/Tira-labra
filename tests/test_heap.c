#include "unity.h" /* I have downloaded unity from http://www.throwtheswitch.org/unity/ */
#include "a_star.h"
#include "heap.h"
#include "constants.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define TEST_HEAPNESS(h) do {					\
	int i;										\
	for (i=1; i < h->n_of_components; i++) {	\
	  char message[30];													\
	  TEST_ASSERT_MESSAGE(h->components[parent(i)].value < h->components[i].value, message); \
	}											\
  } while (0)

void test_new_heap(void) {
  HEAP *h = new_heap();
  TEST_ASSERT_EQUAL_INT(INITIAL_HEAP_SIZE, h->size);
  TEST_ASSERT_EQUAL_INT(0, h->n_of_components);
  delete_heap(h);
}

void test_insert_components(void) {
  HEAP *h = new_heap();
  int i,j;
  for (i=0, j=100; i<100; i++,j--) {
	insert_component(h, i, (double)j);
	TEST_ASSERT_EQUAL_INT(i, get_first_index(h));
  }
  delete_heap(h);
}

void test_remove_first(void) {
  HEAP *h = new_heap();
  int i,j;
  for (i=0, j=100; i<100; i++,j--) {
	insert_component(h, i, (double)j);
  }
  for (i=1; i<=100; i++) {
	TEST_HEAPNESS(h);
	TEST_ASSERT_EQUAL_INT(i, get_first_value(h));
	remove_first(h);
  }
  delete_heap(h);
}

void test_parent_child(void) {
  TEST_ASSERT_EQUAL_INT(0, parent(0));
  TEST_ASSERT_EQUAL_INT(0, parent(1));
  TEST_ASSERT_EQUAL_INT(0, parent(2));
  
  TEST_ASSERT_EQUAL_INT(1, parent(3));
  TEST_ASSERT_EQUAL_INT(1, parent(4));
  TEST_ASSERT_EQUAL_INT(2, parent(5));
  TEST_ASSERT_EQUAL_INT(2, parent(6));

  TEST_ASSERT_EQUAL_INT(1, left_child(0));
  TEST_ASSERT_EQUAL_INT(2, right_child(0));

  TEST_ASSERT_EQUAL_INT(3, left_child(1));
  TEST_ASSERT_EQUAL_INT(4, right_child(1));
  TEST_ASSERT_EQUAL_INT(5, left_child(2));
  TEST_ASSERT_EQUAL_INT(6, right_child(2));
}

void test_swap(void) {
  HEAP *h = new_heap();
  insert_component(h, 1, 1.0);
  insert_component(h, 2, 2.0);
  TEST_ASSERT_EQUAL_INT(1, h->components[0].index);
  TEST_ASSERT_EQUAL_INT(2, h->components[1].index);
  swap(h, 0, 1);
  TEST_ASSERT_EQUAL_INT(2, h->components[0].index);
  TEST_ASSERT_EQUAL_INT(1, h->components[1].index);
  delete_heap(h);
}

void test_heap_new_value(void) {
  HEAP *h = new_heap();
  insert_component(h, 1, 1.0);
  insert_component(h, 2, 2.0);
  TEST_ASSERT_EQUAL_INT(1, h->components[0].index);
  TEST_ASSERT_EQUAL_INT(2, h->components[1].index);
  heap_new_value(h, 0, 5.0);
  TEST_ASSERT_EQUAL_INT(2, h->components[0].index);
  TEST_ASSERT_EQUAL_INT(1, h->components[1].index);
  delete_heap(h);
}

int main(void) {
	printf("\n");
	UNITY_BEGIN();
	RUN_TEST(test_new_heap);
	RUN_TEST(test_insert_components);
	RUN_TEST(test_remove_first);
	RUN_TEST(test_parent_child);
	RUN_TEST(test_swap);
	RUN_TEST(test_heap_new_value);
	return UNITY_END();
}
