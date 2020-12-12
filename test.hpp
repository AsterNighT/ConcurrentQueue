#pragma once
#include "queue.hpp"

using QueueType = MutexNonBlockListQueue;
class Tester {
   public:
    static auto runTest() -> int;
    static const size_t testSize;
    static const size_t consumerSize;
    static const size_t producerSize;
    static const size_t consumeGap;
    static const size_t produceGap;

   private:
    static auto checkResult() -> int;
    static auto producer(size_t start) -> int;
    static auto consumer() -> int;
    static DataType* consumption;
    static std::atomic_int producerCount;
    static std::atomic_int consumerCount;
    static bool count[];
    static ConQueue* testQueue;
};
