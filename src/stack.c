#include <stack.h>

struct stack *create_stack(struct stack *stack, int init_size) {
  struct stack *temp_stack;
  temp_stack = (struct stack *)malloc(sizeof(struct stack));
  init_stack(temp_stack, init_size);
  return temp_stack;
}

void init_stack(struct stack *stack, int init_size) {
  stack->stack_pointer = 0;
  stack->size = init_size;
  stack->stack_array = (word_t *)malloc((size_t)stack->size * sizeof(word_t));
}

void push(struct stack *stack, word_t value) {
	if(stack->stack_pointer >= stack->size) {
		stack->size++;
		stack->stack_array = (word_t *)realloc(stack->stack_array, (size_t)stack->size * sizeof(word_t));
	}
	stack->stack_array[stack->stack_pointer++] = value;
}

word_t pop(struct stack *stack) {
  if(stack->stack_pointer > 0) {
  stack->size--;
  return stack->stack_array[--stack->stack_pointer];
  } else {
    printf("not enough elements on the stack");
    exit(0);
  }
}

void add(struct stack *stack) {
  uint32_t x, y, z;
  if(stack->stack_pointer >= 2) {
    x = (uint32_t)pop(stack);
    y = (uint32_t)pop(stack);
    z = x + y;
    push(stack, (word_t)z);
  } else {
    printf("not enough elements on the stack");
    exit(0);
  }

}

void sub(struct stack *stack) {
  if(stack->stack_pointer >= 2) {
    word_t x = pop(stack);
    word_t y = pop(stack);
    word_t z = y - x;
    push(stack, z);
  } else {
    printf("not enough elements on the stack");
    exit(0);
  }
}

void and(struct stack *stack) {
  if(stack->stack_pointer >= 2) {
    word_t x = pop(stack);
    word_t y = pop(stack);
    word_t z = x & y;
    push(stack, z);
  } else {
    printf("not enough elements on the stack");
    exit(0);
  }

}

void or(struct stack *stack) {
  if(stack->stack_pointer >= 2) {
    word_t x = pop(stack);
    word_t y = pop(stack);
    word_t z = x | y;
    push(stack, z);
  } else {
    printf("not enough elements on the stack");
    exit(0);
  }
}

void swap(struct stack *stack) {
  if(stack->stack_pointer >= 2) {
    word_t x = pop(stack);
    word_t y = pop(stack);
    push(stack, x);
    push(stack, y);
  } else {
    printf("not enough elements on the stack");
    exit(0);
  }

}
