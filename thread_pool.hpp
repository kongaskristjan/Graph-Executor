#ifndef __THREAD_POOL_HPP__
#define __THREAD_POOL_HPP__

#include <thread_pool_job.hpp>
#include <memory>

/*
  Thread_pool:
  push() - push a job to be processed
  wait() - wait for all jobs to complete
*/

class Thread_pool {
public:
    virtual void push(std::unique_ptr<Thread_pool_job>) = 0;
    virtual void wait() = 0;
    
    virtual inline ~Thread_pool();
};

inline Thread_pool::~Thread_pool()
{}

#endif
