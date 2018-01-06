#include "qtensor/core/thread_pool.h"

#include <cassert>

namespace qtensor {
namespace thread_pool {

thread_local ThreadRandomEngine ThreadRandomEngine::engine_;

Task::Task(const std::function<void()>& task, uint32_t ref)
  : task_(task), ref_count_(ref), run_(true) {}

void Task::Run() {
  if (run_.load() && run_.exchange(false)) {
    task_();
  }
  UnRef();
}

void Task::UnRef() {
  if (--ref_count_ == -1) {
    delete this;
  }
}

TaskQueue::TaskQueue(uint32_t size)
  : tasks_(new std::atomic<Task*>[size]), limit_(size - kMinRemaining),
    size_(size), count_(0), consumer_ptr_(0), producer_ptr_(0),
    waiting_slot_(nullptr) {
    assert(size_ > kMinRemaining * 2);
    for (uint32_t i = 0; i < size; i++) {
      tasks_[i].store(nullptr);
    }
  }

Task* TaskQueue::GetTask() {
  std::atomic<Task*>& slot = tasks_[consumer_ptr_++ % size_];
  for (uint32_t i = 0; i < kSpinWait; i++) {
    if (slot.load() != nullptr) {
      break;
    }
  }
  if (slot.load() == nullptr) {
    std::unique_lock<std::mutex> lock(mu_);
    waiting_slot_.store(&slot);
    if (slot.load() == nullptr) {
      cv_.wait(lock, [&]{ return slot.load() != nullptr; });
    }
    waiting_slot_.store(nullptr);
  }
  count_--;
  return slot.exchange(nullptr);
}

bool TaskQueue::AppendTask(Task* task) {
  if (++count_ > limit_) {
    count_--;
    return false;
  }
  std::atomic<Task*>& slot = tasks_[producer_ptr_++ % size_];
  slot.store(task);
  if (waiting_slot_.load() == &slot) {
    std::unique_lock<std::mutex> lock(mu_);
    cv_.notify_all();
  }
  return true;
}

ThreadPool::ThreadPool(uint32_t thread_num,
                      uint32_t queue_size,
                      uint32_t backup)
  : thread_num_(thread_num), backup_(std::min(backup, thread_num)),
    stop_(new bool[thread_num]) {
  for (uint32_t i = 0; i < thread_num_; i++) {
    queues_.emplace_back(new TaskQueue(queue_size / thread_num * backup_));
  }
  for (uint32_t i = 0; i < thread_num_; i++) {
    threads_.emplace_back([this, i]{
      ThreadLoop(i);
    });
  }
}

ThreadPool::~ThreadPool() {
  for (uint32_t i = 0; i < thread_num_; i++) {
    Task* task = new Task([this, i]{
        stop_[i] = true;
    }, 1);
    while (!queues_[i]->AppendTask(task));
  }
  for (uint32_t i = 0; i < thread_num_; i++) {
    threads_[i].join();
  }
}

bool ThreadPool::Schedule(const std::function<void()>& task) {
  ThreadRandomEngine::Reset(thread_num_);
  Task* raw_task = new Task(task, backup_);
  uint32_t counter = 0;
  ThreadRandomEngine::Reset(thread_num_);
  for (uint32_t i = 0; i < backup_; i++) {
    uint32_t id = ThreadRandomEngine::NextRandom();
    if (queues_[id]->AppendTask(raw_task)) {
      counter++;
    } else {
      raw_task->UnRef();
    }
  }
  return counter != 0;
}

void ThreadPool::ThreadLoop(uint32_t id) {
  while (!stop_[id]) {
    Task* task = queues_[id]->GetTask();
    task->Run();
  }
}

}
}

