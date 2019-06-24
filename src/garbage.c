#include <garbage.h>

void mark(struct heap_element *heap_element) {
  if(heap_element->marked == true) {
    return;
  }
  if(heap_element->array != NULL){
    heap_element->marked = true;
  }  
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
