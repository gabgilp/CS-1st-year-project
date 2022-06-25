#include <ijvm.h>
#include <stdlib.h>
#include "stack.h"
#include "basic_helper.h"

unsigned int program_counter;
byte_t *bytes;
byte_t *first_byte;
block_t constant_pool;
block_t text;
stack_t the_stack;

int init_ijvm(char *binary_file)
{
  FILE *binary = fopen(binary_file, "rb");
  if(binary == NULL){
    return -1;
  }
  program_counter = 0;

  fseek(binary, 0, SEEK_END);
  int size = ftell(binary);
  fseek(binary, 0, SEEK_SET);

  bytes = (byte_t*) malloc(size * sizeof(byte_t));

  fread(bytes, sizeof(byte_t), size, binary);
  
  first_byte = bytes;

  word_t first_word = bytes_to_word_M(first_byte);
  

  if(first_word != MAGIC_NUMBER){
    printf("File provided is not an IJVM file.\n");
    return -1;
  }

  //Remove comment to print the file byte by byte for debbuging 
  //print_full_file(bytes, size);

  word_t constant_pool_origin = bytes_to_word_M(first_byte + 4);
  word_t constant_pool_size = bytes_to_word_M(first_byte + 8);

  constant_pool = get_constant_block(constant_pool_origin, constant_pool_size, first_byte + 12);
  
  word_t text_origin = bytes_to_word_M(first_byte + 12 + constant_pool_size);
  word_t text_size = bytes_to_word_M(first_byte + 16 + constant_pool_size);

  text = get_text_block(text_origin, text_size, first_byte + 20 + constant_pool_size);

  return 0;
}

void destroy_ijvm()
{
  free(bytes);
}

void run()
{
  //printf for debugging, remove later
  //printf("\n\n");
  while(step());
}

void set_input(FILE *fp)
{
  // TODO: implement me
}

void set_output(FILE *fp)
{
  // TODO: implement me
}

int get_program_counter(void) {
  return program_counter;
}

int text_size(void){
  return text.size;
}

byte_t *get_text(void){
  return text.block_starting_byte;
}

//TODO
word_t tos(void){
  return 0;
}


//functions that help with step
//these functions should be moved to new file after test2 works
bool interpret_instruction(byte_t instruction[], unsigned int *pc){
  switch(instruction[*pc]){
    case OP_BIPUSH:
      printf("BIPUSH\n");
      *pc += 2;
      break;
    case OP_DUP:
      printf("DUP\n");
      *pc += 1;
      break;
    case OP_ERR:
      printf("ERR\n");
      return false;
      break;
    case OP_GOTO:
      printf("GOTO\n");
      //the pc is incremented now but when functionality is added it should updated accordingly
      *pc += 3;
      break;
    case OP_HALT:
      printf("HALT\n");
      return false;
      break;
    case OP_IADD:
      printf("IADD\n");
      *pc += 1;
      break;
    case OP_IAND:
      printf("IAND\n");
      *pc += 1;
      break;
    case OP_IFEQ:
      printf("IFEQ\n");
      //the pc is incremented now but when functionality is added it should updated accordingly
      *pc += 3;
      break;
    case OP_IFLT:
      printf("IFLT\n");
      //the pc is incremented now but when functionality is added it should updated accordingly
      *pc += 3;
      break;
    case OP_ICMPEQ:
      printf("ICMPEQ\n");
      //the pc is incremented now but when functionality is added it should updated accordingly
      *pc += 3;
      break;
    case OP_IINC:
      printf("IINC\n");
      *pc += 3;
      break;
    case OP_ILOAD:
      printf("ILOAD\n");
      *pc += 2;
      break;
    case OP_IN:
      printf("IN\n");
      *pc += 1;
      break;
    case OP_INVOKEVIRTUAL:
      printf("INVOKEVIRTUAL\n");
      *pc += 3;
      break;
    case OP_IOR:
      printf("IOR\n");
      *pc += 1;
      break;
    case OP_IRETURN:
      printf("IRETURN\n");
      *pc += 1;
      break;
    case OP_ISTORE:
      printf("ISTORE\n");
      *pc += 2;
      break;
    case OP_ISUB:
      printf("ISUB\n");
      *pc += 1;
      break;
    case OP_LDC_W:
      printf("LDC_W\n");
      *pc += 3;
      break;
    case OP_NOP:
      printf("NOP\n");
      *pc += 1;
      break;
    case OP_OUT:
      printf("OUT\n");
      *pc += 1;
      break;
    case OP_POP:
      printf("POP\n");
      *pc += 1;
      break;
    case OP_SWAP:
      printf("SWAP\n");
      *pc += 1;
      break;
    case OP_WIDE:
      printf("WIDE\n");
      *pc += 1;
      break;
  }
  return true;
}

//TODO
bool step(void){
  if (program_counter >= text.size){
    return false;
  }
  return interpret_instruction(text.block_starting_byte, &program_counter);
}