#include "queue.hpp"
#include "test.hpp"
auto main() -> int {
    auto q = new MutexNonBlockListQueue();
    Tester::runTest(q);
    auto q2 = new MutexNonBlockListQueue();
    Tester::runTest(q2, 1000000, 1, 1);

    return 0;
}