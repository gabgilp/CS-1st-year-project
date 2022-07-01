#ifndef FRAMES_HELPER_H
#define FRAMES_HELPER_H

#include "ijvm.h"
#include "stack.h"
#include <stdlib.h>

typedef struct frame{
    struct frame *previous_frame;
    int previous_program_counter;
    stack_t *current_stack;
    word_t *current_locals;
}frame_t;

void init_frame(int previous_pc);

void destroy_frame();

#endif