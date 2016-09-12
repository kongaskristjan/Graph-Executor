#ifndef __GRAPH_EXECUTOR_HPP__
#define __GRAPH_EXECUTOR_HPP__

#include <memory>
#include <cstdint>
#include <job.hpp>


/*
  Graph_executor:
  set_async_mode() and async_mode() - control and report
      Graph_executor's permission to run asyncronously (but
      it is not obliged to run asyncronously)
  
  push() - push a job to be processed or
      a result to be used for initialization or to fill space
  
  sync(i) - force Graph_executor to finish all jobs j < i.
      Will not return before everything is done.
      If i is not specified, sync everything submitted.
  clear(i) - release all results j < i as soon as possible
      If i is not specified, clear everything submitted that is independent.
  
  operator[] - return result. In general avoid storing the reference/pointer,
      but if you must, don't clear before the reference is destroyed.
      If answer is not yet calculated, it will wait until it is calculated.
  hand_over() - hand result over to the main process. If answer is
      not yet calculated, it will wait until it is calculated.

  Graph_executor assumes that no threads are explicitly created by any
      jobs. Jobs may however be created by main thread or other jobs.
*/


class Graph_executor {
public:
    virtual void set_async_mode(bool) = 0;
    virtual bool async_mode() const = 0;
    
    virtual uint64_t push(std::unique_ptr<Job> job,
                          const std::vector<uint64_t> & args) = 0;
    virtual uint64_t push(std::unique_ptr<Result>) = 0;
    
    virtual void sync() = 0;
    virtual void sync(uint64_t) = 0;
    virtual void clear() = 0;
    virtual void clear(uint64_t) = 0;
    
    virtual const Result & operator[](uint64_t) = 0;
    virtual std::unique_ptr<Result> hand_over(uint64_t) = 0;
    
    virtual inline ~Graph_executor();
};

inline Graph_executor::~Graph_executor()
{}

#endif
