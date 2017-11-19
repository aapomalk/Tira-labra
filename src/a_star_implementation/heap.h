#ifndef HEAP_H
#define HEAP_H

int parent(int heap_index);
int left_child(int heap_index);
int right_child(int heap_index);
void swap(HEAP *h, int id1, int id2);
void heapify(HEAP *h, int heap_index);

#endif
