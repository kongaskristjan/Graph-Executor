
#include <perform_tests.hpp>

void Perform_tests::all(Tester & tester, const size_t n)
{
    exponent(tester, n);
    fibonacci(tester, n);
    random_add(tester, n);
}


void Perform_tests::exponent(Tester & tester, const size_t n)
{
    for (size_t i = 1; i <= n; ++i)
        tester.push(Example_exp(3, i), std::vector<uint64_t>());
    
    tester.verify_and_end_test();
}


void Perform_tests::fibonacci(Tester & tester, const size_t n)
{
    tester.push(Integer(0));
    tester.push(Integer(1));
    
    for (size_t i = 2; i <= n; ++i)
        tester.push(Add(), std::vector<uint64_t>({i - 2, i - 1}));
    
    tester.verify_and_end_test();
}


void Perform_tests::random_add(Tester & tester, const size_t n, const size_t k)
{
    auto now = std::chrono::system_clock::now();
    unsigned seed = std::chrono::duration_cast<std::chrono::nanoseconds>
        (now.time_since_epoch()).count();
    std::default_random_engine gen(seed);
    
    for (size_t i = 0; i < k; ++i)
        tester.push(Integer(i + 1));

    for (size_t i = k; i < k + n; ++i){
        std::uniform_int_distribution<uint64_t> distr(0, i - 1);

        uint64_t idx0 = distr(gen);
        uint64_t idx1 = distr(gen);
        tester.push(Add(), std::vector<uint64_t>({idx0, idx1}));
    }
    
    tester.verify_and_end_test();
}
