#include "frames_helper.h"
#include "ijvm.h"
#include "stack.h"
#include "instructions_helper.h"
#include <stdlib.h>



void init_frame(int previous_pc){
    frame_t *new_frame = (frame_t*) malloc(sizeof(frame_t));
    stack_t *new_stack = (stack_t*) malloc(sizeof(stack_t));
    new_stack->size = 0;
    word_t *new_locals = (word_t*) malloc(sizeof(word_t) * local_variables_size);

    new_frame->previous_frame = current_frame;
    new_frame->previous_program_counter = previous_pc;
    new_frame->current_stack = new_stack;
    new_frame->current_locals = new_locals;

    the_stack = new_frame->current_stack;
    local_variables = new_frame->current_locals;
    current_frame = new_frame;
}

void destroy_frame(){
    int size = the_stack->size;
    for (int i = 0; i < size; i++) {
        pop(the_stack);
    }

    the_stack = current_frame->previous_frame->current_stack;
    local_variables = current_frame->previous_frame->current_locals;
    
    frame_t* temp = current_frame;

    current_frame = current_frame->previous_frame;

    free(temp->current_stack);
    free(temp->current_locals);

    free(temp);
}