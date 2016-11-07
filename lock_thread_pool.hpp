#ifndef __LOCK_THREAD_POOL_HPP__
#define __LOCK_THREAD_POOL_HPP__

#include <cassert>
#include <vector>
#include <memory>
#include <typeinfo>
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <thread_pool_job.hpp>
#include <thread_pool.hpp>

/*
  Thread_pool_jobs pushed will be stored in a lockable vector.

  Dead lock is eliminated by ensuring only one active lock per thread
  at all times.
*/


class Lock_thread_pool: public Thread_pool {
public:
    Lock_thread_pool(uint64_t = 0); // pass 0 to detect hardware concurrency
    ~Lock_thread_pool();

    void push(std::unique_ptr<Thread_pool_job>);
    void finish();

private:
    std::unique_ptr<Thread_pool_job> get_job();
    void thread_wait();
    inline void check_invariant() const;

    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<Thread_pool_job>> jobs;
    std::condition_variable worker_cv, finish_cv;
    std::mutex jobs_mtx;

    std::atomic<uint64_t> undone_jobs{0}; // number of jobs not yet done
    bool finish_flag{false};
    bool exit_flag{false};
};


inline void Lock_thread_pool::check_invariant() const
{
    assert(! (exit_flag && finish_flag));
}

#endif
