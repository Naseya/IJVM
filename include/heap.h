#ifndef HEAP_H
#define HEAP_H

#include <ijvm.h>
#include <stdlib.h>
#include <unistd.h>

#include <stack.h>

#define OP_NEWARRAY ((byte_t) 0xD1)
#define OP_IALOAD   ((byte_t) 0xD2)
#define OP_IASTORE  ((byte_t) 0xD3)

struct heap {
  word_t heap_size;
  struct heap_element **location; //pointer to the pointer of the location of the array
};

struct heap_element {
  word_t count; //must be of type int?
  word_t *array;
}

//void init_heap(void);







#endif
