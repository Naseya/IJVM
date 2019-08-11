#include <frame.h>

void init_frame(struct frame *frame, int program_counter_, int frame_pointer, int amount_vars) {
  frame->program_counter = program_counter_;
  frame->frame_pointer = frame_pointer;
  frame->amount_vars = amount_vars; //local_vars
  frame->local_vars = (word_t *)malloc((size_t)(amount_vars) * sizeof(word_t));
  frame->next_frame = NULL;
}

struct frame *head_frame(struct frame *first_frame) { //head of ll
  struct frame *head = first_frame;
  while(head->next_frame != NULL) {
    head = head->next_frame;
  }
  return head;
}

struct frame *create_frame(int program_counter_, int frame_pointer, int local_vars) {
  struct frame *temp_frame;
  temp_frame = (struct frame *)malloc(sizeof(struct frame));
  init_frame(temp_frame, program_counter_, frame_pointer, local_vars);
  return temp_frame;
}
