#include <heap.h>

void init_heap(struct heap *heap, int init_size) {
  heap->heap_size = init_size;
  heap->element = NULL;
}
