
#include <perform_thread_pool_tests.hpp>

Tp_power::Tp_power(std::atomic<uint64_t> & _sum, int _xp):
    sum(_sum), xp(_xp)
{}


void Tp_power::execute()
{
    // Arithmetic modulo 2^64
    uint64_t m = 1;
    for (int i = 0; i < xp; ++i)
        m *= 3;
    sum += 1;
}


uint64_t power_test(Thread_pool & tp)
{
    std::atomic<uint64_t> sum(0);
    for (int i = 0; i < 1e4; ++i)
        tp.push(std::make_unique<Tp_power>(sum, 10));
    tp.finish();
    
    return sum;
}


void perform_all_tests(Thread_pool_tester & tester)
{
    tester.test(power_test, "Powering test");
}
