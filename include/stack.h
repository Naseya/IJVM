#ifndef STACK_H
#define STACK_H

#include <ijvm.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct stack {
  int stack_pointer;
  int size;
  word_t *stack_array;
};

//initialize memory for the stack
struct stack *create_stack(struct stack *stack, int init_size);

//give the stack initial parameters
void init_stack(struct stack *stack, int init_size);

//push a value to the stack
void push(struct stack *stack, word_t value);

//pop a value from the stack and return it
word_t pop(struct stack *stack);

//pop two values from the stack, add them together, 
//pushes result back on the stack
void add(struct stack *stack);

//pop two values from the stack, substract them from eachother, 
//pushes result back on the stack
void sub(struct stack *stack);

//pop two values from the stack logical AND them and push the result back
void and(struct stack *stack);

//pop two values from the stack logical OR them and push the result back
void or(struct stack *stack);

//pop two values from the stack and pushes them back in different order
void swap(struct stack *stack);

#endif
