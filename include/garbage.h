#ifndef GARBAGE_H
#define GARBAGE_H

#include <ijvm.h>
#include <stdlib.h>
#include <unistd.h>
#include <heap.h>
#include <stack.h>
#include <string.h>

#define OP_GARBAGE ((byte_t) 0xD4)

struct heap *gc(struct stack *stack, struct heap *heap, struct frame *first_frame);

void collect(struct stack *stack, struct heap *heap, struct frame *first_frame);

void collect_stack(struct stack *stack, struct heap *heap);

void collect_frames(struct frame *first_frame, struct heap *heap);

void destroy(struct heap *heap);

struct heap *reset_heap(struct heap *heap);

void destroy_tmp_heap(struct heap *new_heap);

void reset_marks(struct heap *heap);

#endif