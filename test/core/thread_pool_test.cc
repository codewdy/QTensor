#include "gtest/gtest.h"
#include "qtensor/core/thread_pool.h"

#include <atomic>
#include <thread>
#include <iostream>

using qtensor::ThreadPool;

TEST(ThreadPool, ThreadPool) {
  std::atomic<uint32_t> x(0);
  const uint32_t kCount = 1 << 16;
  std::atomic<uint32_t> kOk(0);
  {
    ThreadPool pool(4, 1 << 10);
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; i++) {
      threads.emplace_back([&](){
        for (uint32_t j = 0; j < kCount; j++) {
          if (pool.Schedule([&]{
            x++;
          })) {
            kOk++;
          }
        }
      });
    }
    for (auto& thread : threads) {
      thread.join();
    }
    while (x.load() < kOk) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
  ASSERT_EQ(kOk, x.load());
}

