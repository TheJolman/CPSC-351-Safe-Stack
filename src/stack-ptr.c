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

// Creating the stack node to hold an integer value
typedef struct StackNode { 
  int data; 
  struct StackNode* next; 
} StackNode; 

// Constant variables for the program
const int NUM_THREADS = 200;
StackNode* top = NULL; // Global variable for the stack top 
pthread_mutex_t mutex; // Mutex to avoid race condition

// Function to push a value onto the stack 
void push(int value) {
  pthread_mutex_lock(&mutex); // Lock the mutex 
  StackNode* newNode = (StackNode*)malloc(sizeof(StackNode)); // New node to add a value onto the stack
  newNode->data = value; 
  newNode->next = top;
  top = newNode; 
  pthread_mutex_unlock(&mutex); // Unlock the mutex
}

// Function to pop a value onto the stack
int pop() {
  pthread_mutex_lock(&mutex); // Lock the mutex
  
  if (top == NULL) { 
    pthread_mutex_unlock(&mutex); // Unlock the mutex if stack is empty 
    return -1; 
  } 
  
  StackNode* tempNode = top; 
  int value = top->data; 
  top = top->next; 
  free(tempNode); 
  pthread_mutex_unlock(&mutex); // Unlock the mutex 
  return value;
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
  
    // Print the popped values 
    if (val1 != -1) printf("Popped: %d\n", val1);
    if (val2 != -1) printf("Popped: %d\n", val2); 
    if (val3 != -1) printf("Popped: %d\n", val3); 
    return NULL;
  }
} 

int main() { 
  pthread_t threads[NUM_THREADS]; // Array to hold threads 
  pthread_mutex_init(&mutex, NULL); // Initialize the mutex

  // Create 200 threads to perform testStack
  for (int i = 0; i < NUM_THREADS; i++) { 
    pthread_create(&threads[i], NULL, testStack, NULL);
  } 

  // Wait for all threads to finish executing 
  for (int i = 0; i < NUM_THREADS; i++) { 
    pthread_join(threads[i], NULL); 
  } 
  // Cleanup the mutex
  pthread_mutex_destroy(&mutex); 

  return 0;
}
