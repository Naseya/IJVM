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
#include <garbage.h>
#include <inttypes.h>
#include <limits.h>
#include <frame.h>

struct block {
  uint32_t origin;
  uint32_t size;
  byte_t *data;
};

//Swaps the endianness of the value
uint32_t swap_uint32(uint32_t value);

//Takes in the file pointer and reads the bytes, 
//then assigns the constant block and text block each in their own struct
void read_binary_blocks(FILE *fp);

//Check if the binary begins with DEADFAD
bool check_magic_number(FILE *fp);

//pushes the top of the stack again, duplicating it
void dup_and_push(void);

//pops the top of the stack and prints it to the terminal
void out_and_pop(void);

//take offset, jump to label, -1 to set pc on the next command
void go_to(void);

//takes a byte makes it a short
short short_bytes_signed(void);

//takes a byte makes it an unsigned short
unsigned short_bytes_unsigned(void);

//conditional branch, if popped word is zero
void ifeq(void);

//conditional branch if popped word is less than ero
void iflt(void);

//conditional branch if two popped words are equal
void icmpeq(void);

//convert a byte to a word
word_t convert_byte_to_word(byte_t *byte);

//push value from constantpool to stack
void ldc_w(void);

//pops word from the stack and stores it in the local variables of the frame
void store(int i);

//push a local variable to the stack
void load(void);

//add the constant variable to the local variable of the frame
void iinc(void);

//read a character from input and pushes on stack
void read_in(void);

//Invoke a method, pops a reference and arguments from the stack
//creates a new frame
void invoke_virtual(void);

//return the word of the method back to the stack
//deletes frame
void ireturn(void);

#endif
