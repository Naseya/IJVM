#include <stack.h>

void init_stack(struct stack *stack, int init_size) {
  stack->stack_pointer = 0;
  stack->size = init_size;
  stack->stack_array = (word_t *)malloc((size_t)stack->size);
}

void init_frame(struct frame *frame, int program_counter_, int frame_pointer, int amount_vars) {
  frame->program_counter = program_counter_;
  frame->frame_pointer = frame_pointer;
  frame->amount_vars = amount_vars; //local_vars
  frame->local_vars = (word_t *)malloc((size_t)(amount_vars) * sizeof(word_t));
  frame->next_frame = NULL;
}

void push(struct stack *stack, word_t value) {
	if(stack->stack_pointer >= stack->size) {
		stack->size++;
		stack->stack_array = (word_t *)realloc(stack->stack_array, (size_t)stack->size * sizeof(word_t));
	}
	stack->stack_array[stack->stack_pointer++] = value;
}

word_t pop(struct stack *stack) {
  stack->size--;
  return stack->stack_array[--stack->stack_pointer];
}

void add(struct stack *stack) {
  word_t x, y, z;
  x = pop(stack);
  y = (word_t)pop(stack);
  z = x + y;
  push(stack, z);
}

void sub(struct stack *stack) {
  word_t x = pop(stack);
  word_t y = pop(stack);
  word_t z = y - x;
  push(stack, z);
}

void and(struct stack *stack) {
  word_t x = pop(stack);
  word_t y = pop(stack);
  word_t z = x & y;
  push(stack, z);
}

void or(struct stack *stack) {
  word_t x = pop(stack);
  word_t y = pop(stack);
  word_t z = x | y;
  push(stack, z);
}

void swap(struct stack *stack) {
  word_t x = pop(stack);
  word_t y = pop(stack);
  push(stack, x);
  push(stack, y);
}
