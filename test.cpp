#include "test.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
const size_t Tester::testSize = 10000;
const size_t Tester::consumerSize = 100;
const size_t Tester::consumeGap = 50;
const size_t Tester::producerSize = 100;
const size_t Tester::produceGap = 50;

DataType* Tester::consumption = nullptr;
std::atomic_int producerCount(0);
std::atomic_int consumerCount(0);
DataType count[Tester::producerSize];
ConQueue* testQueue;

auto Tester::producer(size_t start) -> int {
    int t;
    while ((t = producerCount++) < testSize) {
        while (testQueue->put(DataType(start + t)) == -1)
            ;
        std::this_thread::sleep_for(std::chrono::milliseconds(Tester::produceGap));
    }
    return 0;
}

auto Tester::consumer() -> int {
    int t;
    while ((t = consumerCount++) < testSize) {
        DataType tmp;
        while ((tmp = testQueue->get()) == NullValue)
            consumption[t] = tmp;
        std::this_thread::sleep_for(std::chrono::milliseconds(Tester::consumeGap));
    }
    return 0;
}

auto Tester::runTest() -> int {
    // testQueue = new ();
    consumption = new DataType[testSize];
    std::fill(count, count + Tester::producerSize, NullValue);
    auto startTime = std::chrono::system_clock::now();
    std::thread* consumers[consumerSize];
    std::thread* producers[producerSize];
    for (int i = 0; i < producerSize; i++) {
        producers[i] = new std::thread(producer(i * testSize));
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
        size_t pos = consumption[i] / testSize;
        if (count[pos] == NullValue || count[pos] < consumption[i]) {
            count[pos] = consumption[i];
        } else {
            std::cout << "Seem to be an error in producer" << pos << ", value " << count[pos]
                      << " before " << consumption[i] << std::endl;
            return -1;
        }
    }
    std::cout << "Total time: " << elapsedTime.count() << "ms" << std::endl;
    delete[] consumption;
    return 0;
}