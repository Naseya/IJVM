#ifndef GARBAGE_H
#define GARBAGE_H

#include <ijvm.h>
#include <stdlib.h>
#include <unistd.h>
#include <heap.h>
#include <stack.h>
#include <string.h>
#include <frame.h>

#define OP_GARBAGE ((byte_t) 0xD4)

//initialize the new heap without the unused/unreachable arrays
struct heap *gc(struct stack *stack, struct heap *heap, struct frame *first_frame);

//mark all the arrays that are reachable, so we know we cant delete those
void collect(struct stack *stack, struct heap *heap, struct frame *first_frame);

//mark all the reachable arrays in the stack
void collect_stack(struct stack *stack, struct heap *heap);

//mark all the reachable arrays in the frames
void collect_frames(struct frame *first_frame, struct heap *heap);

//check how many arrays are unreachable and determine the new size of the heap
void destroy(struct heap *heap);

//recreate the heap, result is a new heap without the unreachable arrays
struct heap *reset_heap(struct heap *heap);

//unmark all the arrays
void reset_marks(struct heap *heap);

#endif
