#include <ijvm.h>

typedef struct block{
  word_t origin;
  word_t size;
  byte_t *block_starting_byte;
} block_t;

int program_counter;
byte_t *first_byte;
block_t constant_pool;
block_t text;

word_t bytes_to_word (byte_t *byte);
void print_full_file (byte_t bytes[], int size);
block_t get_constant_block (word_t origin, word_t size, byte_t *first_byte);
block_t get_text_block (word_t origin, word_t size, byte_t *first_byte);

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

  word_t first_word = bytes_to_word(first_byte);
  

  if(first_word != MAGIC_NUMBER){
    printf("File is not an IJVM file.\n");
    return -1;
  }

  print_full_file(bytes, size);

  word_t constant_pool_origin = bytes_to_word(first_byte + 4);
  word_t constant_pool_size = bytes_to_word(first_byte + 8);

  constant_pool = get_constant_block(constant_pool_origin, constant_pool_size, first_byte + 12);
  

  word_t text_origin = bytes_to_word(first_byte + 12 + constant_pool_size);
  word_t text_size = bytes_to_word(first_byte + 16 + constant_pool_size);

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

word_t bytes_to_word (byte_t *byte){
  word_t num;

  num = (*byte * 0x01000000) + (*(byte + 1) * 0x010000) + (*(byte + 2) * 0x0100) + (*(byte + 3) * 0x01);
  
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

bool step(void){
  return true;
}