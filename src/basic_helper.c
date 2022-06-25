#include "basic_helper.h"
#include "ijvm.h"

word_t bytes_to_word_M (byte_t *byte){
  word_t num;

  num = (*byte * 0x01000000) + (*(byte + 1) * 0x010000) + (*(byte + 2) * 0x0100) + (*(byte + 3) * 0x01);
  
  return num;
}

word_t byte_to_word_S (byte_t *byte){
  word_t num;

  num = *byte * 0x1;

  return num;
}

void print_full_file (byte_t bytes[], int size){
  printf("The File: \n\n");
  for(int i = 0; i < size; i++){
    printf("%02x ", bytes[i]);

    if(((i + 1)  % 8) == 0){
      printf("\n");
    }
  }
  printf("\n");
}

block_t get_constant_block (word_t origin, word_t size, byte_t *first_byte){
  block_t constant_block;
  
  constant_block.origin = origin;
  constant_block.size = size;
  if (constant_block.size == 0){
    constant_block.block_starting_byte = NULL;
  } else {
    constant_block.block_starting_byte = first_byte;
  }

  return constant_block;
}

block_t get_text_block (word_t origin, word_t size, byte_t *first_byte){
  block_t text_block;

  text_block.origin = origin;
  text_block.size = size;
  text_block.block_starting_byte = first_byte;

  return text_block;
}
