#include <heap.h>

void init_heap(struct heap *heap) {
  heap->heap_size = 0; //array reference at the same time, first array on 0, second on 1 etc.
  heap->heap_element = NULL;//malloc((size_t)heap->heap_size * sizeof(word_t));
}

struct heap_element *new_element(word_t count) {
  struct heap_element *element = malloc(sizeof(struct heap_element));
  element->array = malloc((size_t)count * sizeof(word_t));
  element->reference = clock() * rand();

  return element;
}

void newarray(struct heap *heap, struct stack *stack) {
  struct heap_element *array;
  word_t count = pop(stack);

  array = new_element(count);

  heap->heap_size++; 

  heap->heap_element = realloc(heap->heap_element, (size_t)heap->heap_size * sizeof(word_t *));
  heap->heap_element[heap->heap_size - 1] = array;

  push(stack, array->reference);
  
}

void iastore(struct heap *heap, struct stack *stack) {
  word_t array_ref = pop(stack);
  word_t index = pop(stack) ;
  word_t value = pop(stack);


  heap->heap_element[array_ref]->array[index] = value;
}

void iaload(struct heap *heap, struct stack *stack) {

  word_t array_ref = pop(stack);

  word_t index = pop(stack);

  push(stack, heap->heap_element[array_ref]->array[index]);
}
