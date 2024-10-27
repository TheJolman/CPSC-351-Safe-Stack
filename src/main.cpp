#include <format>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <stack>
#include <vector>


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

  // Prevent copying
  Mutex(const Mutex&) = delete;
  Mutex& operator=(const Mutex&) = delete;
};

/// Mutex lock RAII class
class MutexLock {
private:
  Mutex& mutex;

public:
  MutexLock(Mutex &m) : mutex(m) { mutex.lock(); }
  ~MutexLock() { mutex.unlock(); }
};

/// Thread safe Stack with push and pop operations
template <typename T> class ThreadSafeStack {
private:
  std::stack<T> stack;
  mutable Mutex mutex;

public:
  ThreadSafeStack() = default;

  void push(T value) {
    std::cout << std::format("Pushing {}\n", value);
    MutexLock lock(mutex);
    stack.push(value);
  }

  void pop() {
    if (empty()) {
      std::cout << "Nothing to pop.\n";
    } else {
      T top;
      {
        MutexLock lock(mutex);
        top = stack.top();
        stack.pop();
      }
      std::cout << std::format("Popped {}\n", top);
    }
  }

  bool empty() const {
    MutexLock lock(mutex);
    return stack.empty();
  }
};

#define NUM_PUSHES 3
#define NUM_POPS 3
#define NUM_ITERS 500

template <typename T> void *test_stack(void *arg) {
  auto *stack = static_cast<ThreadSafeStack<T> *>(arg); // I love polymorphism

  for (int i = 0; i < NUM_ITERS; ++i) {
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

