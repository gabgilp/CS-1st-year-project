#include <ijvm.h>
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "basic_helper.h"
#include "instructions_helper.h"

int program_counter;
byte_t *bytes;
byte_t *first_byte;
block_t constant_pool;
block_t text;
stack_t *the_stack;
word_t *stack_array = NULL;

int init_ijvm(char *binary_file)
{
  out = stdout;
  in = stdin;

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
  print_full_file(bytes, size);

  word_t constant_pool_origin = bytes_to_word_M(first_byte + 4);
  word_t constant_pool_size = bytes_to_word_M(first_byte + 8);

  constant_pool = get_constant_block(constant_pool_origin, constant_pool_size, first_byte + 12);
  
  word_t text_origin = bytes_to_word_M(first_byte + 12 + constant_pool_size);
  word_t text_size = bytes_to_word_M(first_byte + 16 + constant_pool_size);

  text = get_text_block(text_origin, text_size, first_byte + 20 + constant_pool_size);

  the_stack = (stack_t*) malloc(sizeof(stack_t));
  the_stack->size = 0;

  local_variables_size = 32;
  local_variables = (word_t*) malloc(sizeof(word_t) * local_variables_size);

  fclose(binary);
  return 0;
}

void destroy_ijvm()
{
  int size = the_stack->size;
  for (int i = 0; i < size; i++) {
    pop(the_stack);
  }
  free(bytes);
  free(stack_array);
  free(the_stack);
  free(local_variables);
}

void run()
{
  while(step());
}

void set_input(FILE *fp)
{
  in = fp;
}

void set_output(FILE *fp)
{
  out = fp;
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

word_t tos(void){
  return the_stack->top->data;
}

bool step(void){
  if (program_counter >= text.size){
    return false;
  }
  return interpret_instruction(text.block_starting_byte, &program_counter, the_stack, &constant_pool);
}

int stack_size(void){
  return the_stack->size;
}

word_t *get_stack(void){
  if(stack_array == NULL){
    stack_array = (word_t*) malloc(sizeof(word_t) * the_stack->size);
  }
  else{
    stack_array = (word_t*) realloc(stack_array, sizeof(word_t) * the_stack->size);
  }
  return to_array(the_stack, stack_array);
}

bool finished(void){
  if(program_counter >= text.size){
    is_finished = true;
  }
  return is_finished;
}

byte_t get_instruction(void){
  return text.block_starting_byte[program_counter];
}

word_t get_constant(int i){
  return bytes_to_word_M(constant_pool.block_starting_byte + (i * 4));
}

word_t get_local_variable(int i){
  return local_variables[i];
}