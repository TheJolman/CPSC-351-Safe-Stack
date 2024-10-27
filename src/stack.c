#include "stack.h"

void push(struct stack_node** top, int data)
{
    struct stack_node* node = (struct stack_node*)malloc(sizeof(struct stack_node));
    
    node->data = data;
    node->next = *top;
    *top = node;    
}

int pop(struct stack_node** top)
{
    if (*top != NULL)
    {
        int data = (*top)->data;
        struct stack_node* next = (*top)->next;

        free(*top);

        *top = next;

        return data;
    }

    printf("Cannot pop empty stack!");
    return -1;
}