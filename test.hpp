#pragma once
#include "queue.hpp"

using QueueType = MutexNonBlockListQueue;
class Tester {
   public:
    static auto runTest(ConQueue* _queue, size_t _testSize = 1000000, size_t _producerSize = 100,
                        size_t _producerGap = 10, size_t _consumerSize = 100,
                        size_t _consumerGap = 10) -> int;

   private:
    static size_t testSize;
    static size_t consumerSize;
    static size_t producerSize;
    static size_t consumerGap;
    static size_t producerGap;
    static auto checkResult() -> int;
    static auto producer(size_t start) -> int;
    static auto consumer() -> int;
    static DataType* consumption;
    static std::atomic_int producerCount;
    static std::atomic_int consumerCount;
    static bool* count;
    static ConQueue* testQueue;
};
