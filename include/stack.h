#ifndef STACK_H
#define STACK_H

#include <ijvm.h>
#include <stdlib.h>
#include <unistd.h>

struct stack {
  int stack_pointer;
  int size;
  word_t *stack_array;
};

struct frame {
  int program_counter;
  int frame_pointer;
  int amount_vars;
  word_t *local_vars;
  struct frame *next_frame;
};


void init_stack(struct stack *stack, int init_size);

void init_frame(struct frame *frame, int program_counter_, int frame_pointer, int amount_vars);

void push(struct stack *stack, word_t value);

word_t pop(struct stack *stack);

void add(struct stack *stack);

void sub(struct stack *stack);

void and(struct stack *stack);

void or(struct stack *stack);

void iout(struct stack *stack);

void swap(struct stack *stack);

#endif
