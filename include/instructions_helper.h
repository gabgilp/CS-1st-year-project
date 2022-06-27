#ifndef INSTRUCTIONS_HELPER_H
#define INSTRUCTIONS_HELPER_H

#include "ijvm.h"
#include "stack.h"
#include "basic_helper.h"

extern FILE * in;
extern FILE * out;
extern bool is_finished;

bool interpret_instruction(byte_t instruction[], int *pc, stack_t *the_stack);

#endif