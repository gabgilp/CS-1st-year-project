#ifndef INSTRUCTIONS_HELPER_H
#define INSTRUCTIONS_HELPER_H

#include "ijvm.h"
#include "stack.h"
#include "basic_helper.h"
#include "instructions_helper.h"
#include "frames_helper.h"

extern FILE *in;
extern FILE *out;
extern bool is_finished;
extern word_t *local_variables;
extern int local_variables_size;
extern frame_t *current_frame;
extern stack_t *the_stack;

bool interpret_instruction(byte_t instruction[], int *pc, block_t *constant_pool);

#endif