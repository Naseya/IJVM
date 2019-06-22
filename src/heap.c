#include <heap.h>

void init_heap(struct heap *heap) {
  heap->heap_size = 0; //array reference at the same time, first array on 0, second on 1 etc.
  heap->heap_element = NULL;//malloc((size_t)heap->heap_size * sizeof(word_t));
}

struct heap_element *new_element(word_t count) {
  struct heap_element *element = malloc(sizeof(struct heap_element));
  element->marked = false;
  element->array = malloc((size_t)count * sizeof(word_t));

  return element;
}

void newarray(struct heap *heap, struct stack *stack) {
  struct heap_element *array;
  word_t count = pop(stack);

  array = new_element(count);

  heap->heap_size++; 

  heap->heap_element = realloc(heap->heap_element, (size_t)heap->heap_size * sizeof(word_t *));

  heap->heap_element[heap->heap_size - 1] = array;

  push(stack, (heap->heap_size - 1));
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

  //printf("final: %d\n", heap->element[array_ref]->heap_array[index]);
  push(stack, heap->heap_element[array_ref]->array[index]);
}
/* 
void mark(struct heap_element *heap_element) {
  if(heap_element->marked == true) {
    return;
  }
  if(heap_element->array != NULL{
    heap_element->marked = true;
  })
  
}

void mark_heap(struct heap *heap) {
  for(word_t i = 0; i < heap->heap_size; i++) {
    mark(heap->heap_element[i]);
  }
}


void sweep(struct heap_element *heap_element) {
  if(heap_element->array == NULL) {
    free(heap_element->array);
    free(heap_element);
  }
}

void sweep_heap(struct heap *heap) {
  for(int i = 0; i < heap->heap_size; i++) {
    sweep(heap->heap_element[i]);
  }
}
*/
