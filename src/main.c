/*
 * CSPSC 351 Group Project 3
 * Date began: 10/25/24
 * Due date: 10/28/24
 * Authors: Joshua Holman, Cameron Rosenthal, Noah Scott, Alejandro Silva
 * 
 * This program: Safe Stack
 *     Program uses mutex locks to perform pushing and popping values onto a stack
 *      in a concurrent enviornment
 * This file:  main.c // I am not sure if this needs to be renamed to stack-ptr.c
 */

/*
 * References to help code: 
 * https://learncplusplus.org/how-to-use-stdthread-with-stdvector-in-multi-thread-c-operations/
*/

#include <iostream> 
#include <vector>
#include <thread>
#include <mutex>

typedef struct StackNode { 
      int data; 
      struct StackNode* next; 
} StackNode; 

// Constant variables for the program
#define NUM_THREADS = 200;
StackNode* top = NULL; // Global variable for the stack top 
std::mutex mtx; // Global mutex to avoid race condition

// Function to push a value onto the stack 
void push(int value) {
  
} 

// Function to pop a value from the stack 
int pop() { 
  
} 

// Function for each thread to perform push and pop operations 500 times
void testStack() { 
  for (int i = 0; i < 500; i++) { 
    // 3 push functions
    push(i); // Push value onto stack 
    push(i + 1); // Push another value 
    push(i + 2); // Push another value 
    
    // 3 pop functions
    int val1 = pop(); // Pop a value 
    int val2 = pop(); // Pop another value 
    int val3 = pop(); // Pop another value 
} 

int main() { 
  std::vector<std::thread> thread_vec; // Vector to hold threads 

  // Create 200 threads to perform testStack
  for (int i = 0; i < NUM_THREADS; i++) { 
    thread_vec.push_back(std::thread(&testStack));
  } 

  // Wait for all threads to finish executing 
  for (auto&t : thread_vec) { 
    t.join(); 
  } 
system("pause")

return 0;
}
