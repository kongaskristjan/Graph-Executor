#ifndef __LOCK_FREE_THREAD_POOL_HPP__
#define __LOCK_FREE_THREAD_POOL_HPP__

#include <thread_pool.hpp>
#include <concurrent_list.hpp>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <exception>
#include <cassert>
#include <memory>
#include <vector>

/*
  Lock_free_thread_pool's algorithm becomes blocking only if there are
  less jobs than threads available, in which case those threads go to sleep.
  Also, calling finish() function induces some blocking behaviour.
*/

class Lock_free_thread_pool: public Thread_pool {
public:
    Lock_free_thread_pool(uint64_t = 0); // 0 to detect hardware concurrency
    ~Lock_free_thread_pool();

    void push(std::unique_ptr<Thread_pool_job>);
    void finish();

private:
    void thread_wait();
    Concurrent_list<std::unique_ptr<Thread_pool_job>>::ptr get_job();
    inline void check_invariant() const;

    std::vector<std::thread> threads;
    Concurrent_list<std::unique_ptr<Thread_pool_job>> jobs;
    std::atomic<int64_t> undone_jobs{0};
    // undone_jobs is kept always at least as large as number of jobs not
    // yet done. It is necessary, as jobs may also push new jobs.

    std::condition_variable_any empty_cv;
    std::condition_variable_any finish_cv;
    std::shared_timed_mutex mtx; // Exclusive access only when jobs is empty

    bool finish_flag = 0;
    bool exit_flag = 0;
    
};


inline void Lock_free_thread_pool::check_invariant() const
{
    assert(! (exit_flag && finish_flag));
    assert(undone_jobs >= 0);
}

#endif
