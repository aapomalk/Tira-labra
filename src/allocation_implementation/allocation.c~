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

void * allocation_realloc(int previous, int new, size_t size, void *ptr) {
  void * ptr2;
  while (new > previous) {
	if ((ptr2 = realloc(ptr, new * size)) == NULL) {
	  printf("trying smaller realloc\n");
	  new = previous + (new - previous) / 2; /* lets check if smaller realloc works */
	} else {
	  break; /* realloc worked */
	}
  }
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
