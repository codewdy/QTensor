#ifndef QTENSOR_CORE_THREAD_POOL_
#define QTENSOR_CORE_THREAD_POOL_

#include <memory>
#include <functional>
#include <thread>
#include <atomic>
#include <vector>
#include <condition_variable>
#include <random>

namespace qtensor {

namespace thread_pool {

class Task {
 public:
  Task(const std::function<void()>& task, uint32_t ref);
  void Run();
  void UnRef();
 private:
  std::function<void()> task_;
  std::atomic<int32_t> ref_count_;
  std::atomic<bool> run_;
};

class TaskQueue {
 public:
  TaskQueue(uint32_t size);
  Task* GetTask();
  bool AppendTask(Task* task);
 private:
  static const uint32_t kMinRemaining = 4;
  static const uint32_t kSpinWait = 100000;

  std::unique_ptr<std::atomic<Task*>[]> tasks_;
  uint32_t limit_;
  uint32_t size_;
  std::atomic<uint32_t> count_;
  uint64_t consumer_ptr_;
  std::atomic<uint64_t> producer_ptr_;

  std::mutex mu_;
  std::condition_variable cv_;
  std::atomic<std::atomic<Task*>*> waiting_slot_;
};

class ThreadRandomEngine {
 public:
  static void Reset(uint32_t size) {
    engine_.n_ = engine_.m_ = size;
    if (engine_.random_slots_.size() == size) {
      return;
    }
    engine_.random_slots_.clear();
    for (uint32_t i = 0; i < size; i++) {
      engine_.random_slots_.push_back(i);
    }
  }
  static uint32_t NextRandom() {
    if (engine_.m_ == 0) {
      return 0;
    }
    uint32_t x = engine_.internal_() % engine_.m_;
    std::swap(engine_.random_slots_[x], engine_.random_slots_[engine_.m_ - 1]);
    return engine_.random_slots_[(engine_.m_--) - 1];
  }
 private:
  ThreadRandomEngine()
    : internal_(std::random_device()()), n_(0), m_(0) {}
  static thread_local ThreadRandomEngine engine_;
  std::mt19937 internal_;
  std::vector<uint32_t> random_slots_;
  uint32_t n_, m_;
};

class ThreadPool {
 public:
  ThreadPool(uint32_t thread_num,
             uint32_t queue_size = 1 << 20,
             uint32_t backup = 3);
  ~ThreadPool();
  bool Schedule(const std::function<void()>& task);
 private:
  void ThreadLoop(uint32_t id);
  uint64_t thread_num_;
  uint32_t backup_;
  std::vector<std::thread> threads_;
  std::vector<std::unique_ptr<TaskQueue>> queues_;
  std::unique_ptr<bool[]> stop_;
};

}

using thread_pool::ThreadPool;

}

#endif

