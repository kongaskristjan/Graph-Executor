
#include <perform_tests.hpp>

void Perform_tests::all(Tester & tester)
{
    fibonacci(tester);
}


void Perform_tests::fibonacci(Tester & tester, const int n)
{
    // Generate Fibonacci numbers
    
    std::vector<uint64_t> idxs;
    idxs.push_back(tester.push(Integer(0)));
    idxs.push_back(tester.push(Integer(1)));
    
    for (int i = 2; i <= n; ++i)
        idxs.push_back(tester.push(Add(), std::vector<uint64_t>
                                   ({idxs[i - 2], idxs[i - 1]})));
    
    tester.verify_and_end_test();
}
