#ifndef HEAP_H
#define STACK_H

#include <ijvm.h>
#include <stdlib.h>
#include <unistd.h>

#define OP_NEWARRAY ((byte_t) 0xD1)
#define OP_IALOAD   ((byte_t) 0xD2)
#define OP_IASTORE  ((byte_t) 0xD3)

struct heap {
  word_t heap_size;
  struct heap_element **element;
};

struct heap_element {
  word_t count;
  word_t *heap_array;
};

void init_heap(struct heap *heap, int init_size);



#endif
