#include <machine.h>

FILE *in;
FILE *out;
static int program_counter;
struct block *constant_pool;
struct block *text;
struct stack *my_stack;
struct frame *first_frame;
struct frame *tmp; //used for free'ing frames

struct heap *my_heap;

bool finish = false;

static uint32_t swap_uint32(uint32_t num) {
  return ((num>>24)&0xff) | ((num<<8)&0xff0000) |((num>>8)&0xff00)| ((num>>8)&0xff00)| ((num<<24)&0xff000000);
}

bool check_magic_number(FILE *fp) {
  word_t magic_number;
  assert(fread(&magic_number, sizeof(word_t), 1, fp) == 1);

  magic_number = (word_t)swap_uint32((uint32_t)magic_number);
  if(magic_number != MAGIC_NUMBER) {
    return false;
  }
  return true;
}

void read_binary_blocks(FILE *fp) {
  constant_pool = (struct block *)malloc(sizeof(struct block));
  text = (struct block *)malloc(sizeof(struct block));

  assert(fread(&constant_pool->origin, sizeof(word_t), 1, fp) == 1);
  assert(fread(&constant_pool->size, sizeof(word_t), 1, fp) == 1);

  constant_pool->size = (word_t)swap_uint32((uint32_t)constant_pool->size);
  constant_pool->data = (byte_t *)malloc((size_t)constant_pool->size);

  if(constant_pool->size > 0) {
    assert(fread(constant_pool->data, 1, (size_t)constant_pool->size, fp) > 0);
  }

  assert(fread(&text->origin, sizeof(word_t), 1, fp) == 1);
  assert(fread(&text->size, sizeof(word_t), 1, fp) == 1);

  text->size = (word_t)swap_uint32((uint32_t)text->size);
  text->data = (byte_t *)malloc((size_t)text->size);

  if(text->size > 0) {
    assert(fread(text->data, 1, (size_t)text->size, fp) > 0);
  }
}

bool step() {
  switch (get_instruction()) {
    case OP_BIPUSH:
      push(my_stack, (int8_t)text->data[program_counter + 1]);
      program_counter++;
      break;
    case OP_DUP:
      dup_and_push();
      break;
    case OP_ERR:
      finish = true;
      return false;
      break;
    case OP_GOTO:
      go_to();
      break;
    case OP_HALT:
      finish = true;
      return false;
      break;
    case OP_IADD:
      add(my_stack);
      break;
    case OP_IAND:
      and(my_stack);
      break;
    case OP_IFEQ:
      ifeq();
      break;
    case OP_IFLT:
      iflt();
      break;
    case OP_ICMPEQ:
      icmpeq();
      break;
    case OP_IINC:
      iinc();
      program_counter += 2;
      break;
    case OP_ILOAD:
      load();
      program_counter++;
      break;
    case OP_IN:
      read_in();
  		break;
  	case OP_INVOKEVIRTUAL:
      invoke_virtual();
		  break;
  	case OP_IOR:
      or(my_stack);
  		break;
		case OP_IRETURN:
      ireturn();
  		break;
  	case OP_ISTORE:
      store(text->data[program_counter + 1]);
  		program_counter++;
  		break;
  	case OP_ISUB:
      sub(my_stack);
  		break;
  	case OP_LDC_W:
      ldc_w();
      program_counter += 2;
  		break;
  	case OP_NOP:
      nop();
  		break;
  	case OP_OUT:
      out_and_pop();
      break;
  	case OP_POP:
      pop(my_stack);
  		break;
  	case OP_SWAP:
      swap(my_stack);
  		break;
  	case OP_WIDE:
      program_counter++;
      if(get_instruction() == OP_ISTORE){
        store(short_bytes_unsigned());
        program_counter += 2;
      }else if(get_instruction() == OP_ILOAD){
        word_t loadvar = get_local_variable(short_bytes_unsigned());
        push(my_stack, loadvar);
        program_counter += 2;
      }
  		break;
    case OP_NEWARRAY:
      printf("NEWARRAY\n");
      break;
    case OP_IALOAD:
      printf("OP_IALOAD\n");
      break;
    case OP_IASTORE:
      printf("OP_IASTORE\n");
      break;


    default:
      return finish = true;
      break;
  }
/*
  for(int i = 0; i < my_stack->size; i++) {
    printf("stack: %d\n", my_stack->stack_array[i]);
  }
*/
  program_counter++;
  return true;
}

void nop() {

}

byte_t get_instruction(void) {
  return text->data[program_counter];
}

int get_program_counter() {
  return program_counter;
}

byte_t *get_text() {
  return text->data;
}

int text_size() {
	return text->size;
}

int init_ijvm(char *binary_file) {
  FILE *fp;
  program_counter = 0;
	if ((fp = fopen(binary_file, "rb")) == NULL) {
		return -1;
  }

  if(check_magic_number(fp) == false) {
    return -1;
  }

  read_binary_blocks(fp);

  set_input(stdin);
  set_output(stdout);

  fclose(fp);

  my_stack = create_stack(0);
  first_frame = create_frame(0,0,0);
  my_heap = create_heap(0);
  return 0;
}

struct stack *create_stack(int init_size) {
  struct stack *temp_stack;
  temp_stack = (struct stack *)malloc(sizeof(struct stack));
  init_stack(temp_stack, init_size);
  return temp_stack;
}

struct frame *create_frame(int program_counter_, int frame_pointer, int local_vars) {
  struct frame *temp_frame;
  temp_frame = (struct frame *)malloc(sizeof(struct frame));
  init_frame(temp_frame, program_counter_, frame_pointer, local_vars);
  return temp_frame;
}

struct heap *create_heap(int init_size) {
  struct heap *temp_heap;
  temp_heap = (struct heap *)malloc(sizeof(struct heap));
  init_heap(temp_heap, init_size);
  return temp_heap;
}

struct frame *head_frame() { //head of ll
  struct frame *head = first_frame;
  while(head->next_frame != NULL) {
    head = head->next_frame;
  }
  return head;
}

word_t *get_stack() {
  return my_stack->stack_array;
}

word_t tos() {
  return my_stack->stack_array[my_stack->stack_pointer - 1];
}

void destroy_ijvm() {
  free(constant_pool->data);
  free(constant_pool);

  free(text->data);
  free(text);

  free(my_stack->stack_array);
  free(my_stack);

  while(first_frame != NULL) {
    tmp = first_frame;
    first_frame = first_frame->next_frame;
    free(tmp->local_vars);
    free(tmp);
  }
}

int stack_size() {
  return my_stack->size;
}

void run(){
  while(step() && program_counter <= text->size);
  finish = true;
}

void set_input(FILE *fp) {
  in = fp;
}

void set_output(FILE *fp) {
  out = fp;
}

void dup_and_push() {
  push(my_stack, tos());
}

void out_and_pop(){
  char x = (char)pop(my_stack);
  fprintf(out, "%c", (char)x);
 }

short short_bytes_signed() {
  short temp = (short)((text->data[program_counter + 1] << 8) | text->data[program_counter + 2]);
  return temp;
}

unsigned short short_bytes_unsigned2(byte_t *byte) {
  unsigned short temp = (unsigned short)((byte[0] << 8) | byte[1]);
  return temp;
}

unsigned short_bytes_unsigned() {
  unsigned short temp = (unsigned short)((text->data[program_counter + 1] << 8) | text->data[program_counter + 2]);
  return temp;
}

void go_to() {
  program_counter += (short_bytes_signed() - 1);
}

void ifeq() {
  if(pop(my_stack) == 0) {
    program_counter += (short_bytes_signed() - 3);
  }
  program_counter += 2;
}

void iflt() {
  if(pop(my_stack) < 0) {
    program_counter += (short_bytes_signed() - 3);
  }
  program_counter += 2;
}

void icmpeq() {
  word_t x = pop(my_stack);
  word_t y = pop(my_stack);
  if(x == y) {
    program_counter += (short_bytes_signed() - 3);
  }
  program_counter += 2;
}

word_t convert_byte_to_word(byte_t *byte) {
  word_t temp = (word_t)((byte[0] << 24) | (byte[1] << 16 ) | (byte[2] << 8) | byte[3]);
	return temp;
}

word_t get_constant(int index) {
  int offset;
  word_t constant;
  if(index < constant_pool->size) {
    offset = index * (int)sizeof(word_t);
    constant = convert_byte_to_word(&constant_pool->data[offset]);
    return constant;
  }
  return -1;
}

void ldc_w() {
  int index = short_bytes_signed();
  word_t constant = get_constant(index);
  push(my_stack, constant);
}

word_t get_local_variable(int i) {
  return head_frame()->local_vars[i];
}

void store(int i) {
  if((i + 1) > head_frame()->amount_vars) {
    head_frame()->amount_vars = (i+1);
    head_frame()->local_vars = realloc(head_frame()->local_vars, (size_t)(head_frame()->amount_vars)*sizeof(word_t));
  }
    head_frame()->local_vars[i] = pop(my_stack);
}

void load() {
  word_t loadvar = get_local_variable(text->data[program_counter + 1]);
  push(my_stack, loadvar);
}

void iinc() {
  int8_t second_byte_constant; //make pedantic ISO C90
  int first_byte_index = text->data[program_counter + 1];;
  second_byte_constant = (int8_t)text->data[program_counter + 2];

  if((first_byte_index + 1) > head_frame()->amount_vars) {
    head_frame()->amount_vars = (first_byte_index+1);
    head_frame()->local_vars = realloc(head_frame()->local_vars, (size_t)(head_frame()->amount_vars)*sizeof(word_t));
  }
    head_frame()->local_vars[first_byte_index] += (int8_t)second_byte_constant;
}

void read_in() {
  int in_ = fgetc(in);
  if(in_ != EOF) {
    push(my_stack, in_);
  } else {
    push(my_stack, 0);
  }
}

void invoke_virtual() {
  int old_program_counter, amount_of_arguments, amount_of_local_variables, tmp;
  struct frame *frame;

  old_program_counter = program_counter + 2;
  program_counter = get_constant(short_bytes_signed()) - 1;

  amount_of_arguments = short_bytes_unsigned();

  program_counter += 2;

  amount_of_local_variables = short_bytes_unsigned();

  program_counter += 2;

  tmp = amount_of_arguments + amount_of_local_variables;

  frame = create_frame(old_program_counter, my_stack->stack_pointer - amount_of_arguments, tmp);

  while(amount_of_arguments > 0) {
    frame->local_vars[amount_of_arguments - 1] = pop(my_stack);
    amount_of_arguments--;
  }

  head_frame()->next_frame = frame;
}

void ireturn() {
  word_t return_value = pop(my_stack);
  struct frame *head = first_frame;

  while(my_stack->stack_pointer > head_frame()->frame_pointer) {
    pop(my_stack);
  }

  program_counter = head_frame()->program_counter;

  while(head->next_frame->next_frame != NULL) {
    head = head->next_frame;
  }

  free(head->next_frame->local_vars);
  free(head->next_frame);
  head->next_frame = NULL;

  push(my_stack, return_value);
}

bool finished() {
  return finish;
}