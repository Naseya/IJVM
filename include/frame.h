#ifndef FRAME_H
#define FRAME_H

#include <ijvm.h>
#include <stack.h>

struct frame {
  int program_counter;
  int frame_pointer;
  int amount_vars;
  word_t *local_vars;
  struct frame *next_frame;
};

//initialize memory for the frame
struct frame *create_frame(int program_counter, int frame_pointer, int local_vars);

//give the frame its parameters
void init_frame(struct frame *frame, int program_counter_, int frame_pointer, int amount_vars);

//give the head of the linkedlist consisting of frames. 
//this represents the method we are currently in
struct frame *head_frame(struct frame *frame);


#endif
