#include "instructions_helper.h"
#include "basic_helper.h"
#include "stack.h"
#include "ijvm.h"

bool interpret_instruction(byte_t instruction[], int *pc, stack_t *the_stack){

  short_t offset;
  word_t num1;
  word_t num2;

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
      return false;
      break;

    case OP_GOTO:
      printf("GOTO\n");
      offset = bytes_to_short(&instruction[(*pc) + 1]);
      *pc += (int) offset;
      break;

    case OP_HALT:
      printf("HALT\n");
      return false;
      break;

    case OP_IADD:
      printf("IADD\n");
      if(the_stack->size < 2){
        printf("Error popping from stack, not enough stored values\n");
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
        return false;
      }
      num1 = pop(the_stack);
      num2 = pop(the_stack);
      push(the_stack, (num1 & num2));
      *pc += 1;
      break;

    case OP_IFEQ:
      printf("IFEQ\n");
      if(pop(the_stack) == 0){
        offset = bytes_to_short(&instruction[(*pc) + 1]);
        *pc += (int) offset;
        break;
      }
      *pc += 3;
      break;

    case OP_IFLT:
      printf("IFLT\n");
      if(pop(the_stack) < 0){
        offset = bytes_to_short(&instruction[(*pc) + 1]);
        *pc += (int) offset;
        break;
      }
      *pc += 3;
      break;

    case OP_ICMPEQ:
      printf("ICMPEQ\n");
      if(pop(the_stack) == pop(the_stack)){
        offset = bytes_to_short(&instruction[(*pc) + 1]);
        *pc += (int) offset;
        break;
      }
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
      if(the_stack->size < 2){
        printf("Error popping from stack, not enough stored values\n");
        return false;
      }
      num1 = pop(the_stack);
      num2 = pop(the_stack);
      push(the_stack, (num1 | num2));
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
      if(the_stack->size < 2){
        printf("Error popping from stack, not enough stored values\n");
        return false;
      }
      num1 = pop(the_stack);
      num2 = pop(the_stack);
      push(the_stack, (num2 - num1));
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
      num1 = pop(the_stack);
      printf("%c", num1);
      *pc += 1;
      break;

    case OP_POP:
      printf("POP\n");
      pop(the_stack);
      *pc += 1;
      break;

    case OP_SWAP:
      printf("SWAP\n");
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
  }
  return true;
}