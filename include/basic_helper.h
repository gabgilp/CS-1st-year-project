#ifndef BASIC_HELPER_H
#define BASIC_HELPER_H

#include "ijvm.h"

typedef int16_t short_t;

typedef struct block{
  word_t origin;
  word_t size;
  byte_t *block_starting_byte;
} block_t;


word_t bytes_to_word_M (byte_t *byte);

word_t byte_to_word_S (byte_t *byte);

short_t bytes_to_short (byte_t *byte);

void print_full_file (byte_t bytes[], int size);

block_t get_constant_block (word_t origin, word_t size, byte_t *first_byte);

block_t get_text_block (word_t origin, word_t size, byte_t *first_byte);

#endif
