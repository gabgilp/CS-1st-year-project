#ifndef INSTRUCTIONS_HELPER_H
#define INSTRUCTIONS_HELPER_H

#include "ijvm.h"
#include "stack.h"
#include "basic_helper.h"

bool interpret_instruction(byte_t instruction[], unsigned int *pc, stack_t *the_stack);

#endif