#ifndef __SINGLE_THREAD_POOL_HPP__
#define __SINGLE_THREAD_POOL_HPP__

#include <memory>
#include <queue>
#include <cassert>
#include <thread_pool_job.hpp>
#include <thread_pool.hpp>

/*
  Single_thread_pool is mainly for testing purposes.
*/

class Single_thread_pool: Thread_pool {
public:
    void push(std::unique_ptr<Thread_pool_job>);
    void wait();

private:
    void execute();
    inline void check_invariant() const;
    
    std::queue<std::unique_ptr<Thread_pool_job> > jobs;
    bool executing = false;
};


inline void Single_thread_pool::check_invariant() const
{}

#endif
