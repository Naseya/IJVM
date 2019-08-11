#ifndef HEAP_H
#define HEAP_H

#include <ijvm.h>
#include <stdlib.h>
#include <unistd.h>
#include <stack.h>
#include <limits.h>
#include <string.h>
#include <time.h>


#define OP_NEWARRAY ((byte_t) 0xD1)
#define OP_IALOAD   ((byte_t) 0xD2)
#define OP_IASTORE  ((byte_t) 0xD3)

struct heap {
  word_t heap_size;
  struct heap_element **heap_element;
};

struct heap_element {
  bool marked;
  word_t reference;
  word_t *array;
};

//initialize memory for the heap
struct heap *create_heap(struct heap *heap);

//give the heap initial parameters
void init_heap(struct heap *heap);

//initialize memory for an new array in the heap
struct heap_element *new_element(word_t count);

//create new array on the heap
void newarray(struct heap *my_heap, struct stack *my_stack);

//store a value on a specific place in the array on the heap
void iastore(struct heap *my_heap, struct stack *my_stack);

//load a value from a specific place in the array on the heap and push that to the stack
void iaload(struct heap *my_heap, struct stack *my_stack);

#endif
