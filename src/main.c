#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include "stack.h"
#include <unistd.h>

const int PUSH_MAX = 500;
const int PUSH_MIN = 1;

static int thread_num = 0;
struct stack_node *top = NULL;

void* stack_test(void* arg)
{
    printf("Thread Num: %d\n", thread_num++);

    // struct stack_node* top = *(struct stack_node**)arg;
    
    for (int i = PUSH_MIN; i <= PUSH_MAX; i++)
    {   
        push(&top, i);
    }
    
    for (int i = PUSH_MAX; i >= PUSH_MIN; i--)
    {   
        int value = pop(&top);

        if (i != value)
            printf("WARNING: Expected: %d, Received: %d\n", i, value);
    }
    
    pthread_exit(NULL);
}


int main()
{
    const int THREAD_COUNT = 5000;

    pthread_t thread_list[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++)
    {   
        pthread_create(&thread_list[i], NULL, &stack_test, NULL); 
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(thread_list[i], NULL); 
    }

    return 0;
}