/*
 * CSPSC 351 Group Project 3
 * Date began: 10/25/24
 * Due date: 10/28/24
 * Authors: Joshua Holman, Cameron Rosenthal, Noah Scott, Alejandro Silva
 *      in a concurrent enviornment
 * This file:  main.c // I am not sure if this needs to be renamed to stack-ptr.c
 */
#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 

typedef struct StackNode { 
      int data; 
      struct StackNode* next; 
} StackNode; 

// Constant variables for the program
const int NUM_THREADS = 200;
StackNode* top = NULL; // Global variable for the stack top 
pthread_mutex_t mtx; // Mutex to avoid race condition

// Function to push a value onto the stack 
void push(int value) {
}

// Function to pop a value onto the stack
int pop() {
} 

// Function for each thread to perform push and pop operations 500 times
void* testStack(void* arg) { 
  for (int i = 0; i < 500; i++) { 
    push(i); // Push value onto stack 
    push(i + 1); // Push another value 
    push(i + 2); // Push another value 

    int val1 = pop(); // Pop a value 
    int val2 = pop(); // Pop another value 
    int val3 = pop(); // Pop another value 
  }
} 

int main() { 
  pthread_t threads[NUM_THREADS]; // Array to hold threads 
  pthread_mutex_init(&mtx, NULL); // Initialize the mutex

  // Create 200 threads to perform testStack
  for (int i = 0; i < NUM_THREADS; i++) { 
    pthread_create(&threads[i], NULL, testStack, NULL);
  } 

  // Wait for all threads to finish executing 
  for (int i = 0; i < NUM_THREADS; i++) { 
    pthread_join(threads[i], NULL); 
  } 
  // Cleanup the mutex
  pthread_mutex_destroy(&mtx); 

return 0;
}
