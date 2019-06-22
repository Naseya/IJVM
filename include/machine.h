#ifndef MACHINE_H
#define MACHINE_H
#include <ijvm.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include <stack.h>
#include <assert.h>
#include <heap.h>
#include <network.h>

struct block {
  word_t origin;
  word_t size;
  byte_t *data;
};
struct frame *head_frame(void);

uint32_t swap_uint32(uint32_t num);

struct block *read_block(FILE *fp);

void read_binary_blocks(FILE *fp);

struct stack *create_stack(int init_size);

struct frame *create_frame(int program_counter, int frame_pointer, int local_vars);

struct heap *create_heap();

bool check_magic_number(FILE *fp);

void dup_and_push(void);

void out_and_pop(void);

void go_to(void);

short short_bytes_signed(void);

unsigned short_bytes_unsigned(void);

void ifeq(void);

void iflt(void);

void icmpeq(void);

word_t convert_byte_to_word(byte_t *byte);

unsigned short short_bytes_unsigned2(byte_t *byte);

void ldc_w(void);

void store(int i);

void load(void);

void iinc(void);

void read_in(void);

void invoke_virtual(void);

void ireturn(void);

void nop(void);

#endif
