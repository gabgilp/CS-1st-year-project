#include "stack.h"
#include "ijvm.h"
#include <stdlib.h>

void push(stack_t *the_stack, word_t word){
    node_t *new_node = (node_t*) malloc(sizeof(node_t));
    new_node->data = word;

    if(the_stack->size == 0){
        the_stack->top = new_node;
        new_node->next =NULL;
        the_stack->size++;
        printf("Pushed into stack: %08x\n", word);
        return;
    }

    new_node->next = the_stack->top;
    the_stack->top = new_node;
    the_stack->size++;
    printf("Pushed into stack: %08x\n", word);
    return;
}

word_t pop(stack_t *the_stack){
    word_t temp_word;
    node_t *temp_node;

    if(the_stack->size == 1){
        temp_word = the_stack->top->data;
        temp_node = the_stack->top;
        the_stack->top = NULL;
        the_stack->size--;
        printf("Popped from stack: %08x\n", temp_word);

        free(temp_node);
        return temp_word;
    }

    temp_word = the_stack->top->data;
    temp_node = the_stack->top;
    the_stack->top = the_stack->top->next;
    the_stack->size--;
    printf("Popped from stack: %08x\n", temp_word);

    free(temp_node);
    return temp_word;
}