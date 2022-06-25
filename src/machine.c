#include <ijvm.h>
#include "stack.h"
#include "basic_helper.h"

int program_counter;
byte_t *first_byte;
block_t constant_pool;
block_t text;

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

  byte_t bytes[size];

  fread(bytes, sizeof(byte_t), size, binary);
  
  first_byte = bytes;

  word_t first_word = bytes_to_word_M(first_byte);
  

  if(first_word != MAGIC_NUMBER){
    printf("File is not an IJVM file.\n");
    return 1;
  }

  //Remove comment to print the file for debbuging 
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
  // Reset IJVM state
}

void run()
{
  step();
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

word_t tos(void){
  return 0;
}

bool step(void){
  return true;
}
