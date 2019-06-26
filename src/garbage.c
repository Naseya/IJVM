#include <garbage.h>

int new_heap_size;

struct heap *gc(struct stack *stack, struct heap *heap, struct frame *first_frame) {
  new_heap_size = heap->heap_size;
  struct heap *new_heap;

  collect(stack, heap, first_frame);
  destroy(heap);
  new_heap = reset_heap(heap);
  reset_marks(new_heap);
  //printf("check ref: %d\n", heap->heap_element[0]->reference);

  return new_heap;
}

void reset_marks(struct heap *heap) {
  for(int i = 0; i < heap->heap_size; i++) {
    heap->heap_element[i]->marked = false;
  }
}


struct heap *reset_heap(struct heap *heap) {
  struct heap *new_heap = malloc((size_t)new_heap_size * sizeof(struct heap));
  new_heap->heap_element = malloc((size_t)new_heap_size * sizeof(word_t *));
  new_heap->heap_size = new_heap_size;

  for(int i = 0; i < new_heap_size; i++) {
    for(int j = 0; j < heap->heap_size; j++) {
      if(heap->heap_element[j]->marked == true) {
        new_heap->heap_element[i] = heap->heap_element[j];
        heap->heap_element[j]->marked = false;
      }
    }
  }

  for(int i = 0; i < heap->heap_size; i++) {
    free(heap->heap_element[i]->array);
    free(heap->heap_element[i]);
  }

  free(heap->heap_element);
  free(heap);

  return new_heap;
}

void destroy(struct heap *heap) {
  for(int i = 0; i < heap->heap_size; i++) {
    if(heap->heap_element[i]->marked == false) { new_heap_size--;}
  }
}

void collect(struct stack *stack, struct heap *heap, struct frame *first_frame) {
  collect_stack(stack, heap);
  collect_frames(first_frame, heap);
}

void collect_stack(struct stack *stack, struct heap *heap) {
  for(int i = 0; i < stack->size; i++) {
    for(int j = 0; j < heap->heap_size; j++) {
      if(stack->stack_array[i] == heap->heap_element[j]->reference) {
        heap->heap_element[i]->marked = true;
      }
    }
  }
}

void collect_frames(struct frame *first_frame, struct heap *heap) {
  struct frame *root = first_frame;
  while(root->next_frame != NULL) {
    for(int j = 0; j < heap->heap_size; j++) {
    for(int i = 0; i < root->amount_vars; i++) {
        if(root->local_vars[i] == heap->heap_element[j]->reference) {
          heap->heap_element[j]->marked = true; 
        }
      }
    }
    root = root->next_frame;
  }

  for(int i = 0; i < root->amount_vars; i++) {
    for(int j = 0; j < heap->heap_size; j++) {
      if(root->local_vars[i] == heap->heap_element[j]->reference) {
        heap->heap_element[i]->marked = true; 
      }
    }
  }
}