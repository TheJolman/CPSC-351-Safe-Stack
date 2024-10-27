#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

struct stack_node
{
    int data;
    struct stack_node *next;
};

void push(struct stack_node** top, int data);

int pop(struct stack_node** top);

#endif