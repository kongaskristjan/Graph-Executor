#ifndef __LOCK_THREAD_POOL_HPP__
#define __LOCK_THREAD_POOL_HPP__

#include <vector>
#include <memory>
#include <typeinfo>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <thread_pool_job.hpp>
#include <thread_pool.hpp>

/*
  Thread_pool_jobs pushed will be stored in a lockable linked list.
  Use better implementations of Thread_pool for better performance.
*/


class Lock_thread_pool: public Thread_pool {
public:
    void push(std::unique_ptr<Thread_pool_job>) = 0;
    void wait() = 0;

    Lock_thread_pool(uint64_t);
    ~Lock_thread_pool();

private:
    void thread_wait();
    
    std::vector<std::thread> threads;
    std::condition_variable cv;
    std::mutex mtx;
};

#endif
