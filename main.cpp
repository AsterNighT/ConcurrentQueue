#include "queue.hpp"
#include "test.hpp"
auto main() -> int {
    // auto q1 = new MutexNonBlockListQueue();
    // Tester::runTest(q1, 10000);
    // auto q2 = new MutexNonBlockListQueue();
    // Tester::runTest(q2, 10000, 1, 10, 1, 10); // No, too slow
    // auto q3 = new MutexBlockListQueue();
    // Tester::runTest(q3, 100000);
    // auto q4 = new CasNonBlockListQueue();
    // Tester::runTest(q4, 10000,100,10,100,10);
    auto q5 = new MutexBlockArrayQueue(100);
    Tester::runTest(q5, 10000, 100, 10, 100, 10);
    return 0;
}