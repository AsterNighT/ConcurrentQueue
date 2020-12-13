#include "test.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
size_t Tester::testSize = 1000000;
size_t Tester::consumerSize = 100;
size_t Tester::consumeGap = 0;
size_t Tester::producerSize = 100;
size_t Tester::produceGap = 0;

DataType* Tester::consumption = nullptr;
std::atomic_int Tester::producerCount(0);
std::atomic_int Tester::consumerCount(0);
bool* Tester::count = nullptr;
ConQueue* Tester::testQueue = nullptr;

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

auto Tester::checkResult() -> int {
    for (int i = 0; i < testSize; i++) {
        size_t pos = consumption[i] % testSize;
        if (!count[pos]) {
            count[pos] = true;
        } else {
            std::cout << "Seem to be an error, " << pos << " is popped twice by " << consumption[i]
                      << std::endl;
            return -1;
        }
    }
    return 0;
}

auto Tester::runTest(ConQueue* _queue, size_t _testSize, size_t _consumerSize, size_t _producerSize)
    -> int {
    testQueue = _queue;
    testSize = _testSize;
    consumerSize = _consumerSize;
    producerSize = _producerSize;
    consumption = new DataType[testSize];
    count = new bool[testSize];
    consumerCount = 0;
    producerCount = 0; 
    std::fill(count, count + testSize, false);
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
    checkResult();

    std::cout << "Total time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count()
              << "ms with " << _producerSize << " producers and " << _consumerSize << " consumers."
              << std::endl;
    delete[] consumption;
    delete[] count;
    return 0;
}