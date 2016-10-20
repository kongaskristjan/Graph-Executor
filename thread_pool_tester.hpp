#ifndef __THREAD_POOL_TESTER_HPP__
#define __THREAD_POOL_TESTER_HPP__

#include <iostream>
#include <chrono>
#include <vector>
#include <memory>
#include <cassert>
#include <thread_pool.hpp>

class Thread_pool_tester {
public:
    void add(std::unique_ptr<Thread_pool>);
    void test(uint64_t (* test)(Thread_pool &), const std::string & name);

private:
    std::vector<std::unique_ptr<Thread_pool> > thread_pools;
};

#endif
