#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <stdlib.h>
#include <stdio.h>

void * allocation_malloc(int number_of_elements, size_t size);
void * allocation_realloc(int previous, void *ptr, int new, size_t size);
void * allocation_calloc(int number_of_elements, size_t size);

#endif
