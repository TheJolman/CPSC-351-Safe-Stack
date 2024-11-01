#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include "stack.h"
#include <unistd.h>

const int TEST_ITERATIONS = 1000;

struct stack_node *top = NULL;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* stack_test(void* arg)
{
    for (int i = 0; i < TEST_ITERATIONS; i++)
    {   
        pthread_mutex_lock(&mutex);

        push(&top, i);
        int value = pop(&top);

        pthread_mutex_unlock(&mutex);

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