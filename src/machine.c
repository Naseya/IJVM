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

uint32_t swap_uint32(uint32_t num) {
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
      //printf("BIPUSH\n");
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
      //printf("HALT\n");
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

      //printf("INVOKE\n");
      invoke_virtual();
		  break;
  	case OP_IOR:
      or(my_stack);
  		break;
		case OP_IRETURN:
      //printf("IRETURN\n");
      ireturn();
  		break;
  	case OP_ISTORE:
      //printf("STORE\n");
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
      //printf("POP\n");
      pop(my_stack);
  		break;
  	case OP_SWAP:
      swap(my_stack);
  		break;
  	case OP_WIDE:
      program_counter++;
      if(get_instruction() == OP_ISTORE){
        store((int)short_bytes_unsigned());
        program_counter += 2;
      }else if(get_instruction() == OP_ILOAD){
        word_t loadvar = get_local_variable((int)short_bytes_unsigned());
        push(my_stack, loadvar);
        program_counter += 2;
      }
  		break;
    case OP_NEWARRAY:
      //printf("NEWARRAY\n");
      
      newarray(my_heap, my_stack);
      //printf("start\n");
      break;
    case OP_IALOAD:
      //printf("IALOAD\n");
      iaload(my_heap, my_stack);
      break;
    case OP_IASTORE:
      //printf("IASTORE\n");
      iastore(my_heap, my_stack);
      break;
    case OP_GARBAGE:
      //printf("GARBAGEBOIIIIII\n");
      my_heap = gc(my_stack, my_heap, first_frame);
      
      break;
    case OP_NETBIND:
      //printf("NETBIND\n");
      netbind(my_stack);
      break;
    case OP_NETCONNECT:
      //printf("NETCONNECT\n");
      netconnect(my_stack);
      break;
    case OP_NETIN:
      //printf("NETIN\n");
      netin(my_stack);
      break;
    case OP_NETOUT:
      //printf("NETOUT\n");
      netout(my_stack);
      break;
    case OP_NETCLOSE:
      //printf("NETCLOSE\n");
      netclose();
      break;      


    default:
      return finish = true;
      break;
  }
  

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

  my_stack = (struct stack*)create_stack(0);
  first_frame = (struct frame*)create_frame(0,0,0);
  my_heap = (struct heap*)create_heap();
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

struct heap *create_heap() {
  struct heap *temp_heap;
  temp_heap = (struct heap *)malloc(sizeof(struct heap));
  init_heap(temp_heap);
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

  for(int i = 1; i < my_heap->heap_size; i++) {
    free(my_heap->heap_element[i]->array);
    free(my_heap->heap_element[i]);
  }

  free(my_heap->heap_element);
  free(my_heap);

  program_counter = 0;
}

int stack_size() {
  return my_stack->size;
}

void run(){
  while(step());
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
  short temp = (short)(((uint32_t)text->data[program_counter + 1] << 8) | (uint32_t)text->data[program_counter + 2]);
  return temp;
}

unsigned short short_bytes_unsigned2(byte_t *byte) {
  unsigned short temp = (unsigned short)(((uint32_t)byte[0] << 8) | (uint32_t)byte[1]);
  return temp;
}

unsigned short_bytes_unsigned() {
  unsigned short temp = (unsigned short)(((uint32_t)text->data[program_counter + 1] << 8) | text->data[program_counter + 2]);
  return temp;
}

void go_to() {
  program_counter += ((int)short_bytes_signed() - 1);
}

void ifeq() {
  if(pop(my_stack) == 0) {
    program_counter += ((int)short_bytes_signed() - 3);
  }
  program_counter += 2;
}

void iflt() {
  if(pop(my_stack) < 0) {
    program_counter += ((int)short_bytes_signed() - 3);
  }
  program_counter += 2;
}

void icmpeq() {
  word_t x = pop(my_stack);
  word_t y = pop(my_stack);
  if(x == y) {
    program_counter += ((int)short_bytes_signed() - 3);
  }
  program_counter += 2;
}

word_t convert_byte_to_word(byte_t *byte) {
  word_t temp = (word_t)(((uint32_t)byte[0] << 24) | ((uint32_t)byte[1] << 16 ) | ((uint32_t)byte[2] << 8) | (uint32_t)byte[3]);
	return temp;
}

word_t get_constant(int index) {
  int offset;
  word_t constant;
  if(index < constant_pool->size) {
    offset = index * (int)sizeof(word_t);
    constant = (int)convert_byte_to_word(&constant_pool->data[offset]);
    return constant;
  }
  return -1;
}

void ldc_w() {
  int index = (int)short_bytes_signed();
  word_t constant = (word_t)get_constant(index);
  push(my_stack, (word_t)constant);
}

word_t get_local_variable(int i) {
  return head_frame()->local_vars[i];
}

void store(int i) {
  if((i + 1) > head_frame()->amount_vars) {
    head_frame()->amount_vars = (int)(i+1);
    head_frame()->local_vars = (word_t *)realloc(head_frame()->local_vars, (size_t)(head_frame()->amount_vars)*sizeof(word_t));
  }
    head_frame()->local_vars[i] = (word_t)pop(my_stack);
}

void load() {
  word_t loadvar = (word_t)get_local_variable(text->data[program_counter + 1]);
  push(my_stack, (word_t)loadvar);
}

void iinc() {
  int8_t second_byte_constant; //make pedantic ISO C90
  int first_byte_index = (int)text->data[program_counter + 1];;
  second_byte_constant = (int8_t)text->data[program_counter + 2];

  if((first_byte_index + 1) > head_frame()->amount_vars) {
    head_frame()->amount_vars = (int)(first_byte_index+1);
    head_frame()->local_vars = (word_t *)realloc(head_frame()->local_vars, (size_t)(head_frame()->amount_vars)*sizeof(word_t));
  }
    head_frame()->local_vars[first_byte_index] += (int8_t)second_byte_constant;
}

void read_in() {
  int in_ = (int)fgetc(in);
  if(in_ != EOF) {
    push(my_stack, (int)in_);
  } else {
    push(my_stack, 0);
  }
}

void invoke_virtual() {
  int old_program_counter, amount_of_arguments, amount_of_local_variables, _tmp_;
  struct frame *frame;

  old_program_counter = program_counter + 2;
  program_counter = (int)get_constant(short_bytes_signed()) - 1;

  amount_of_arguments = (int)short_bytes_unsigned();

  program_counter += 2;

  amount_of_local_variables = (int)short_bytes_unsigned();

  program_counter += 2;

  _tmp_ = amount_of_arguments + amount_of_local_variables;

  frame = (struct frame*)create_frame((int)old_program_counter, (int)my_stack->stack_pointer - (int)amount_of_arguments, _tmp_);

  while(amount_of_arguments > 0) {
    frame->local_vars[amount_of_arguments - 1] = (word_t)pop(my_stack);
    amount_of_arguments--;
  }

  head_frame()->next_frame = (struct frame*)frame;
}

void ireturn() {
  word_t return_value = (word_t)pop(my_stack);
  struct frame *head = (struct frame *)first_frame;

  while(my_stack->stack_pointer > head_frame()->frame_pointer) {
    (word_t)pop(my_stack);
  }

  program_counter = (int)head_frame()->program_counter;

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
