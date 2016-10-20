#ifndef __PERFORM_THREAD_POOL_TESTS_HPP__
#define __PERFORM_THREAD_POOL_TESTS_HPP__

#include <thread_pool.hpp>
#include <thread_pool_tester.hpp>
#include <atomic>

class Tp_power: public Thread_pool_job {
public:
    Tp_power(std::atomic<uint64_t> & _sum, int _xp);
    void execute();
    
private:
    std::atomic<uint64_t> & sum;
    int xp;
};

uint64_t power_test(Thread_pool &);

void perform_all_tests(Thread_pool_tester & tester);

#endif
