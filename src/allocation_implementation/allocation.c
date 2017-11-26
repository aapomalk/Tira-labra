#include <stdlib.h>
#include <stdio.h>

void * allocation_malloc(int number_of_elements, size_t size) {
  void * ptr;
  if ((ptr = malloc(number_of_elements * size)) == NULL) {
	printf("allocation (malloc) failed, exiting\n");
	exit(1);
  }
  return ptr;
}

void * allocation_realloc(int previous, void *ptr, int new, size_t size) {
  void * ptr2;
  ptr2 = realloc(ptr, new * size);
  if (ptr2 == NULL) {
	printf("allocation (realloc) failed, exiting\n");
	exit(2);
  }
  return ptr2;
}

void * allocation_calloc(int number_of_elements, size_t size) {
  void *ptr;
  if ((ptr = calloc(number_of_elements, size)) == NULL) {
	printf("allocation (calloc) failed, exiting\n");
	exit(3);
  }
  return ptr;
}
