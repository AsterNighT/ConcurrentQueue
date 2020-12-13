#include "queue.hpp"
#include "test.hpp"
auto main() -> int {
    auto q = new MutexNonBlockListQueue();
    Tester::runTest(q, 10000);
    auto q2 = new MutexNonBlockListQueue();
    Tester::runTest(q2, 10000, 1, 10, 1, 10);

    return 0;
}