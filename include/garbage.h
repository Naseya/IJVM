#ifndef GARBAGE_H
#define GARBAGE_H

#include <ijvm.h>
#include <stdlib.h>
#include <unistd.h>
#include <heap.h>

#define OP_GARBAGE ((byte_t) 0xD4)

void mark_heap(struct heap *heap);

void mark(struct heap_element *heap_element);

void sweep_heap(struct heap *heap);

void sweep(struct heap_element *heap_element);

#endif