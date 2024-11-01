#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include "stack.h"
#include <unistd.h>

const int TEST_ITERATIONS = 500;

struct stack_node *top = NULL;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* stack_test(void* arg)
{
    for (int i = 0; i < TEST_ITERATIONS; i++)
    {   
        pthread_mutex_lock(&mutex);

        push(&top, i);
        push(&top, i + 1);
        push(&top, i + 2);

        int value_3 = pop(&top);
        int value_2 = pop(&top);
        int value_1 = pop(&top);

        if (value_3 != i + 2)
            printf("WARNING: Expected: %d, Received: %d\n", i + 2, value_3);

        if (value_2 != i + 1)
            printf("WARNING: Expected: %d, Received: %d\n", i + 1, value_2);

        if (value_1 != i)
            printf("WARNING: Expected: %d, Received: %d\n", i, value_1);

        pthread_mutex_unlock(&mutex);
    }
    

    pthread_exit(NULL);
}


int main()
{
    const int THREAD_COUNT = 200;

    pthread_t thread_list[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++)
    {   
        pthread_create(&thread_list[i], NULL, &stack_test, NULL); 
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {  
        pthread_join(thread_list[i], NULL); 
    }

    printf("Success!\n");

    return 0;
}