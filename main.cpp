#include "queue.hpp"
#include "test.hpp"
auto main() -> int {
    auto q1 = new MutexNonBlockListQueue();
    Tester::runTest(q1, 10000000, 12, 0, 12, 0);
    // auto q2 = new MutexNonBlockListQueue();
    // Tester::runTest(q2, 10000, 1, 10, 1, 10); // No, too slow
    auto q3 = new MutexBlockListQueue();
    Tester::runTest(q3, 10000000, 12, 0, 12, 0);
    auto q4 = new CasNonBlockListQueue();
    Tester::runTest(q4, 10000000, 12, 0, 12, 0);
    auto q5 = new MutexBlockArrayQueue(100);
    Tester::runTest(q5, 10000000, 12, 0, 12, 0);
    auto q6 = new PaperCasNonBlockListQueue();
    Tester::runTest(q6, 1000000, 12, 0, 12, 0);
    return 0;
}