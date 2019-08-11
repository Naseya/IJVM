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
  if(fread(&magic_number, sizeof(word_t), 1, fp) == 1){} else {
    exit(1);
  };

  magic_number = (word_t)swap_uint32((uint32_t)magic_number);
  if(magic_number != MAGIC_NUMBER) {
    return false;
  }
  return true;
}

void read_binary_blocks(FILE *fp) {
  constant_pool = (struct block *)malloc(sizeof(struct block));
  text = (struct block *)malloc(sizeof(struct block));

  if(fread(&constant_pool->origin, sizeof(word_t), 1, fp) == 1){} else {
    exit(1);
  }
  if(fread(&constant_pool->size, sizeof(word_t), 1, fp) == 1) {} else {
    exit(1);
  }

  constant_pool->size = (word_t)swap_uint32((uint32_t)constant_pool->size);
  constant_pool->data = (byte_t *)malloc((size_t)constant_pool->size);

  if(constant_pool->size > 0) {
    if(fread(constant_pool->data, 1, (size_t)constant_pool->size, fp) > 0) {} else {
      exit(1);
    }
  } 

  if(fread(&text->origin, sizeof(word_t), 1, fp) == 1){} else {
    exit(1);
  }
  if(fread(&text->size, 4, 1, fp) == 1) {} else {
    exit(1);
  }

  text->size = (word_t)swap_uint32((uint32_t)text->size);
  text->data = (byte_t *)malloc((size_t)text->size);

  if(text->size > 0) {
    if(fread(text->data, 1, (size_t)text->size, fp) > 0){} else {
      exit(1);
    }
  }
}

bool step() {
  word_t operation = get_instruction();
  switch (operation) {
    case OP_BIPUSH:
      //printf("BIPUSH\n");
      push(my_stack, (int8_t)text->data[program_counter + 1]);
      program_counter++;
      break;
    case OP_DUP:
      dup_and_push();
      break;
    case OP_ERR:
      return false;
      break;
    case OP_GOTO:
      go_to();
      break;
    case OP_HALT:
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
      newarray(my_heap, my_stack);
      break;
    case OP_IALOAD:
      iaload(my_heap, my_stack);
      break;
    case OP_IASTORE:
      iastore(my_heap, my_stack);
      break;
    case OP_GARBAGE:
      my_heap = gc(my_stack, my_heap, first_frame);
      break;
    case OP_NETBIND:
      netbind(my_stack);
      break;
    case OP_NETCONNECT:
      netconnect(my_stack);
      break;
    case OP_NETIN:
      netin(my_stack);
      break;
    case OP_NETOUT:
      netout(my_stack);
      break;
    case OP_NETCLOSE:
      netclose();
      break;      
    default:
      finish = true;
      break;
  }

  program_counter++;
  return true;
}

byte_t get_instruction() {
  if(text->data != NULL && text->size >= program_counter) {
    return text->data[program_counter];
  } else {
    exit(0);
  }
}

int get_program_counter() {
  if(program_counter >= 0) {
    return program_counter;
  } else {
    exit(0);
  }
  
}

byte_t *get_text() {
  if(text->data != NULL) {
    return text->data;
  } else {
    exit(0);
  }
  
}

int text_size() {
  if(text->size >= 0) {
    return text->size;
  } else {
    exit(0);
  }
	
}

int init_ijvm(char *binary_file) {
  FILE *fp;
  program_counter = 0;
	if ((fp = fopen(binary_file, "rb")) == NULL) {
		exit(0);
  }

  if(check_magic_number(fp) == false) {
    exit(0);
  }

  read_binary_blocks(fp);

  set_input(stdin);
  set_output(stdout);

  fclose(fp);

  my_stack = create_stack(my_stack, 0);
  first_frame = create_frame(0,0,0);
  my_heap = create_heap(my_heap);

  return 0;
}

word_t *get_stack() {
  return my_stack->stack_array;
}

word_t tos() {
  return my_stack->stack_array[my_stack->stack_pointer - 1];
}

void destroy_ijvm() {

  //my_heap = gc(my_stack, my_heap, first_frame);

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
  while(step() && !finished()) {
  };
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
  word_t temp = (word_t)(((uint32_t)byte[0] << 24) | ((uint32_t)byte[1] << 16 ) | ((uint32_t)byte[2] << 8) | (uint32_t)byte[3]);
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
  return head_frame(first_frame)->local_vars[i];
}

void store(int i) {
  if((i + 1) > head_frame(first_frame)->amount_vars) {
    head_frame(first_frame)->amount_vars = (i+1);
    head_frame(first_frame)->local_vars = (word_t *)realloc(head_frame(first_frame)->local_vars, (size_t)(head_frame(first_frame)->amount_vars)*sizeof(word_t));
  }
    head_frame(first_frame)->local_vars[i] = pop(my_stack);
}

void load() {
  word_t loadvar = get_local_variable(text->data[program_counter + 1]);
  push(my_stack, loadvar);
}

void iinc() {
  int8_t second_byte_constant; //make pedantic ISO C90
  int first_byte_index = (int)text->data[program_counter + 1];;
  second_byte_constant = (int8_t)text->data[program_counter + 2];

  if((first_byte_index + 1) > head_frame(first_frame)->amount_vars) {
    head_frame(first_frame)->amount_vars = (first_byte_index+1);
    head_frame(first_frame)->local_vars = (word_t *)realloc(head_frame(first_frame)->local_vars, (size_t)(head_frame(first_frame)->amount_vars)*sizeof(word_t));
  }
    head_frame(first_frame)->local_vars[first_byte_index] += second_byte_constant;
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
  int old_program_counter, amount_of_arguments, amount_of_local_variables, _tmp_;
  struct frame *frame;

  old_program_counter = program_counter + 2;
  program_counter = get_constant(short_bytes_signed()) - 1;

  amount_of_arguments = (int)short_bytes_unsigned();

  program_counter += 2;

  amount_of_local_variables = (int)short_bytes_unsigned();

  program_counter += 2;

  _tmp_ = amount_of_arguments + amount_of_local_variables;

  frame = create_frame(old_program_counter, my_stack->stack_pointer - amount_of_arguments, _tmp_);

  while(amount_of_arguments > 0) {
    frame->local_vars[amount_of_arguments - 1] = pop(my_stack);
    amount_of_arguments--;
  }

  head_frame(first_frame)->next_frame = frame;
}

void ireturn() {
  word_t return_value = pop(my_stack);
  struct frame *head = first_frame;

  while(my_stack->stack_pointer > head_frame(first_frame)->frame_pointer) {
    pop(my_stack);
  }

  program_counter = head_frame(first_frame)->program_counter;

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
