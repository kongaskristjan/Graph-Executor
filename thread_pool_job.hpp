#ifndef __THREAD_POOL_JOB_HPP__
#define __THREAD_POOL_JOB_HPP__

/*
  Thread_pool_job object must contain it's own data.
  You may assume that no copying occurs in Thread_pool.
*/

class Thread_pool_job {
    virtual void execute() = 0;
    virtual ~Thread_pool_job();
};


inline Thread_pool_job::~Thread_pool_job()
{}

#endif
