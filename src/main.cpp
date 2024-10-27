#include <format>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <stack>
#include <vector>

#define NUM_PUSHES 3
#define NUM_POPS 3

/// pthread RAII wrapper class
class Thread {
private:
  pthread_t thread;
  bool joined = false;

public:
  Thread(void *(*start_routine)(void *), void *arg) {
    int ret = pthread_create(&thread, nullptr, start_routine, arg);
    if (ret != 0) {
      throw std::runtime_error(std::format("Pthread_create failed: {}", ret));
    }
  }

  // Prevent copying
  Thread(const Thread &) = delete;
  Thread &operator=(const Thread &) = delete;

  // Move constructor
  Thread(Thread &&other) noexcept : thread(other.thread), joined(other.joined) {
    other.joined = true;
  }

  ~Thread() {
    if (!joined) {
      pthread_join(thread, nullptr);
    }
  }
};

/// Mutex RAII wrapper to be used with MutexLock
class Mutex {
private:
  pthread_mutex_t mutex;

public:
  Mutex() { pthread_mutex_init(&mutex, nullptr); }

  void lock() { pthread_mutex_lock(&mutex); }

  void unlock() { pthread_mutex_unlock(&mutex); }

  ~Mutex() { pthread_mutex_destroy(&mutex); }
};

/// Mutex lock RAII class
class MutexLock {
private:
  Mutex mutex;

public:
  MutexLock(Mutex &m) : mutex(m) { mutex.lock(); }
  ~MutexLock() { mutex.unlock(); }
};

/// Thread safe Stack with push and pop operations
template <typename T> class ThreadSafeStack {
private:
  std::stack<T> stack;
  Mutex mutex;

public:
  ThreadSafeStack() { stack = {}; }

  void push(T value) {
    MutexLock lock(mutex);
    stack.push(value);
  }

  T pop() {
    MutexLock lock(mutex);
    T ret_value = stack.top();
    stack.pop();
    return ret_value;
  }
};

template <typename T> void *test_stack(void *arg) {
  auto *stack = static_cast<ThreadSafeStack<T> *>(arg); // I love polymorphism

  for (int i = 0; i < 500; ++i) {
    for (int i = 0; i < NUM_PUSHES; ++i) {
      std::srand(std::time(nullptr));
      stack->push(std::rand());
    }

    for (int i = 0; i < NUM_POPS; ++i) {
      stack->pop(); // don't actually care about return value
    }
  }
  return nullptr;
}

int main() {

  const size_t num_threads = 200;

  try {
    auto shared_stack = std::make_unique<ThreadSafeStack<int>>();
    std::vector<Thread> threads;
    threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
      // .get() returns raw pointer from smart pointer
      threads.emplace_back(&test_stack<int>, shared_stack.get());
    }
  } catch (std::exception &e) {
    std::cout << std::format("Error: {}\n", e.what());
  }

  return 0;
}

