#include "instructions_helper.h"
#include "basic_helper.h"
#include "stack.h"
#include "ijvm.h"
#include "frames_helper.h"
#include <stdio.h>
#include <stdlib.h>

FILE *in;
FILE *out;
bool is_finished = false;
word_t *local_variables;
int local_variables_size;
frame_t *current_frame;
stack_t *the_stack;

bool interpret_instruction(byte_t instruction[], int *pc, block_t *constant_pool){

  short_t offset;
  short_t index_short;
  byte_t index_byte;
  word_t num1;
  word_t num2;
  short_t argument_number;

  switch(instruction[*pc]){

    case OP_BIPUSH:
      printf("BIPUSH\n");
      num1 = byte_to_word_S(&instruction[(*pc) + 1]);
      push(the_stack, num1);
      *pc += 2;
      break;

    case OP_DUP:
      printf("DUP\n");
      num1 = pop(the_stack);
      push(the_stack, num1);
      push(the_stack, num1);
      *pc += 1;
      break;

    case OP_ERR:
      printf("ERR\n");
      printf("Encountered an error, program stopped.\n");
      is_finished = true;
      return false;
      break;

    case OP_GOTO:
      printf("GOTO\n");
      offset = bytes_to_short(&instruction[(*pc) + 1]);
      *pc += (int) offset;
      break;

    case OP_HALT:
      printf("HALT\n");
      is_finished = true;
      return false;
      break;

    case OP_IADD:
      printf("IADD\n");
      if(the_stack->size < 2){
        printf("Error popping from stack, not enough stored values\n");
        is_finished = true;
        return false;
      }
      num1 = pop(the_stack);
      num2 = pop(the_stack);
      push(the_stack, (num1 + num2));
      *pc += 1;
      break;

    case OP_IAND:
      printf("IAND\n");
      if(the_stack->size < 2){
        printf("Error popping from stack, not enough stored values\n");
        is_finished = true;
        return false;
      }
      num1 = pop(the_stack);
      num2 = pop(the_stack);
      push(the_stack, (num1 & num2));
      *pc += 1;
      break;

    case OP_IFEQ:
      printf("IFEQ\n");
      if(the_stack->size < 1){
        printf("Error popping from stack, not enough stored values\n");
        is_finished = true;
        return false;
      }
      if(pop(the_stack) == 0){
        offset = bytes_to_short(&instruction[(*pc) + 1]);
        *pc += (int) offset;
        break;
      }
      *pc += 3;
      break;

    case OP_IFLT:
      printf("IFLT\n");
      if(the_stack->size < 1){
        printf("Error popping from stack, not enough stored values\n");
        is_finished = true;
        return false;
      }
      if(pop(the_stack) < 0){
        offset = bytes_to_short(&instruction[(*pc) + 1]);
        *pc += (int) offset;
        break;
      }
      *pc += 3;
      break;

    case OP_ICMPEQ:
      printf("ICMPEQ\n");
      if(the_stack->size < 1){
        printf("Error popping from stack, not enough stored values\n");
        is_finished = true;
        return false;
      }
      if(pop(the_stack) == pop(the_stack)){
        offset = bytes_to_short(&instruction[(*pc) + 1]);
        *pc += (int) offset;
        break;
      }
      *pc += 3;
      break;

    case OP_IINC:
      printf("IINC\n");
      num1 = local_variables[instruction[*pc + 1]];
      num2 = byte_to_word_S(&instruction[*pc + 2]);
      local_variables[instruction[*pc + 1]] = num1 + num2;
      *pc += 3;
      break;

    case OP_ILOAD:
      printf("ILOAD\n");
      push(the_stack, local_variables[instruction[*pc + 1]]);
      *pc += 2;
      break;

    case OP_IN:
      printf("IN\n");
      byte_t input = fgetc(in);
      if(input != 0xff){
        push(the_stack, byte_to_word_S(&input));
        *pc += 1;
        break;
      }
      push(the_stack, 0x00);
      *pc += 1;
      break;

    case OP_INVOKEVIRTUAL:
      printf("INVOKEVIRTUAL\n");
      
      index_short = bytes_to_short(&instruction[(*pc) + 1]);
      *pc += 3;
      init_frame(*pc);
      *pc = bytes_to_word_M(constant_pool->block_starting_byte + (index_short * 4));

      argument_number = bytes_to_short(&instruction[*pc]);

      *pc += 4;

      if(current_frame->previous_frame->current_stack->size < argument_number){
        printf("Error popping from stack, not enough stored values\n");
        is_finished = true;
        return false;
      }

      for(int i = 0; i < argument_number; i++){
        num1 = pop(current_frame->previous_frame->current_stack);
        local_variables[argument_number - 1 - i] = num1;
      }

      break;

    case OP_IOR:
      printf("IOR\n");
      if(the_stack->size < 2){
        printf("Error popping from stack, not enough stored values\n");
        is_finished = true;
        return false;
      }
      num1 = pop(the_stack);
      num2 = pop(the_stack);
      push(the_stack, (num1 | num2));
      *pc += 1;
      break;

    case OP_IRETURN:
      printf("IRETURN\n");
      *pc = current_frame->previous_program_counter;
      num1 = pop(the_stack);
      destroy_frame();
      push(the_stack, num1);
      break;

    case OP_ISTORE:
      printf("ISTORE\n");
      if(the_stack->size < 1){
        printf("Error popping from stack, not enough stored values\n");
        is_finished = true;
        return false;
      }
      num1 = pop(the_stack);
      index_byte = instruction[(*pc) + 1];
      while(index_byte >= local_variables_size){
        local_variables_size *= 2;
        local_variables = (word_t*) realloc(local_variables, local_variables_size);
      }
      local_variables[index_byte] = num1;
      printf("Pushed to var%i, value %i\n", index_byte, num1);
      *pc += 2;
      break;

    case OP_ISUB:
      printf("ISUB\n");
      if(the_stack->size < 2){
        printf("Error popping from stack, not enough stored values\n");
        is_finished = true;
        return false;
      }
      num1 = pop(the_stack);
      num2 = pop(the_stack);
      push(the_stack, (num2 - num1));
      *pc += 1;
      break;

    case OP_LDC_W:
      printf("LDC_W\n");
      index_short = bytes_to_short(&instruction[(*pc) + 1]);
      num1 = bytes_to_word_M(constant_pool->block_starting_byte + (index_short * 4));
      push(the_stack, num1);
      *pc += 3;
      break;

    case OP_NOP:
      printf("NOP\n");
      *pc += 1;
      break;

    case OP_OUT:
      printf("OUT\n");
      if(the_stack->size < 1){
        printf("Error popping from stack, not enough stored values\n");
        is_finished = true;
        return false;
      }
      num1 = pop(the_stack);
      fputc(num1, out);
      *pc += 1;
      break;

    case OP_POP:
      printf("POP\n");
      if(the_stack->size < 1){
        printf("Error popping from stack, not enough stored values\n");
        is_finished = true;
        return false;
      }
      pop(the_stack);
      *pc += 1;
      break;

    case OP_SWAP:
      printf("SWAP\n");
      if(the_stack->size < 2){
        printf("Error popping from stack, not enough stored values\n");
        is_finished = true;
        return false;
      }
      num1 = pop(the_stack);
      num2 = pop(the_stack);
      push(the_stack, num1);
      push(the_stack, num2);
      *pc += 1;
      break;

    case OP_WIDE:
      printf("WIDE\n");
      *pc += 1;
      break;

    default:
      is_finished = true;
      return false;
  }
  return true;
}