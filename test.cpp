#include "test.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
const size_t Tester::testSize = 1000000;
const size_t Tester::consumerSize = 100;
const size_t Tester::consumeGap = 0;
const size_t Tester::producerSize = 100;
const size_t Tester::produceGap = 0;

DataType* Tester::consumption = nullptr;
std::atomic_int producerCount(0);
std::atomic_int consumerCount(0);
bool count[Tester::testSize];
ConQueue* testQueue;

auto Tester::producer(size_t start) -> int {
    int t;
    while ((t = producerCount++) < testSize) {
        while (testQueue->put(DataType(start + t)) == -1)
            ;
        std::this_thread::sleep_for(std::chrono::milliseconds(produceGap));
    }
    return 0;
}

auto Tester::consumer() -> int {
    int t;
    while ((t = consumerCount++) < testSize) {
        DataType tmp;
        while ((tmp = testQueue->get()) == NullValue)
            ;
        consumption[t] = tmp;
        std::this_thread::sleep_for(std::chrono::milliseconds(consumeGap));
    }
    return 0;
}

auto Tester::runTest() -> int {
    testQueue = new QueueType();
    consumption = new DataType[testSize];
    std::fill(count, count + producerSize, false);
    auto startTime = std::chrono::system_clock::now();
    std::thread* consumers[consumerSize];
    std::thread* producers[producerSize];
    for (int i = 0; i < producerSize; i++) {
        producers[i] = new std::thread(producer, i * testSize);
    }
    for (int i = 0; i < consumerSize; i++) {
        consumers[i] = new std::thread(consumer);
    }
    for (auto& t : producers) {
        t->join();
    }
    for (auto& t : consumers) {
        t->join();
    }
    auto endTime = std::chrono::system_clock::now();
    auto elapsedTime = endTime - startTime;
    for (int i = 0; i < testSize; i++) {
        size_t pos = consumption[i] % testSize;
        if (!count[pos]) {
            count[pos] = true;
        } else {
            std::cout << "Seem to be an error, " << pos << " is popped twice by " << consumption[i] << std::endl;
            return -1;
        }
    }

    std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count() << "ms"
              << std::endl;
    delete[] consumption;
    return 0;
}