#include <heap.h>

struct heap *create_heap(struct heap *heap) {
  struct heap *temp_heap;
  temp_heap = (struct heap *)malloc(sizeof(struct heap));
  init_heap(temp_heap);
  return temp_heap;
}

void init_heap(struct heap *heap) {
  heap->heap_size = 0; //array reference at the same time, first array on 0, second on 1 etc.
  heap->heap_element = NULL;
}

struct heap_element *new_element(word_t count) {
  struct heap_element *element = calloc(1, sizeof(struct heap_element));
  element->marked = false;
  element->array = calloc((size_t)count, sizeof(word_t));
  element->reference = (word_t)clock();
  return element;
}

void newarray(struct heap *heap, struct stack *stack) {
  struct heap_element *array;
  word_t count = pop(stack);

  array = new_element(count);

  heap->heap_size++; 

  heap->heap_element = realloc(heap->heap_element, (size_t)heap->heap_size * sizeof(word_t*));
  heap->heap_element[heap->heap_size - 1] = array;

  push(stack, (word_t)array->reference);
  
}

void iastore(struct heap *heap, struct stack *stack) {
  if(stack->stack_pointer >= 3) {
    word_t array_ref = pop(stack);
    word_t index = pop(stack);
    word_t value = pop(stack);

    for(int i = 0; i < heap->heap_size; i++) {
      if(heap->heap_element[i]->reference == array_ref) {
        heap->heap_element[i]->array[index] = value;
      }
    }
  } else {
    printf("Not enough elements on the stack\n");
    exit(1);
  }
}

void iaload(struct heap *heap, struct stack *stack) {
  if(stack->stack_pointer >= 2) {
    word_t array_ref = pop(stack);
    word_t index = pop(stack);
    for(int i = 0; i < heap->heap_size; i++) {
      if(heap->heap_element[i]->reference == array_ref) {
        push(stack, heap->heap_element[i]->array[index]);
      }
    }
  } else {
    printf("Not enough elements on the stack\n");
    exit(1);
  }
}
