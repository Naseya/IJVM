#ifndef HEAP_H
#define HEAP_H

#include <ijvm.h>
#include <stdlib.h>
#include <unistd.h>
#include <stack.h>
#include <limits.h>

#define OP_NEWARRAY ((byte_t) 0xD1)
#define OP_IALOAD   ((byte_t) 0xD2)
#define OP_IASTORE  ((byte_t) 0xD3)

struct heap {
  word_t heap_size;
  struct heap_element **heap_element;
};

struct heap_element {
  bool marked;
  word_t *array;
};

void init_heap(struct heap *heap);

struct heap_element *new_element(word_t count);

void newarray(struct heap *my_heap, struct stack *my_stack);

void iastore(struct heap *my_heap, struct stack *my_stack);

void iaload(struct heap *my_heap, struct stack *my_stack);

#endif
