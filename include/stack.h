#ifndef STACK_H
#define STACK_H

#include "ijvm.h"

typedef struct node{
    word_t data;
    struct node *next;
} node_t;

typedef struct stack{
    node_t *top;
    unsigned int size;
} stack_t;

void push(stack_t *the_stack, word_t word);

word_t pop(stack_t *the_stack);

word_t *to_array(stack_t *the_stack, word_t *the_array);

#endif