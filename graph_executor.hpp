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
  
  push_back() - push a job to be processed or
      a result to be used for initialization or to fill space
  
  sync(i) - force Graph_executor to finish all jobs j <= i.
      Will not return before everything is done.
      If i is not specified, sync everything submitted.
  clear(i) - release all results j <= i as soon as possible
      If i is not specified, clear everything submitted that is independent.
  
  operator[] - return result. If answer is not yet calculated,
      wait until it is calculated.
  hand_over() - hand result over to the main process. If answer is
      not yet calculated, wait until it is calculated.
*/


class Graph_executor {
public:
    virtual void set_async_mode(bool) = 0;
    virtual bool async_mode() const = 0;
    
    virtual std::uint64_t push_back(std::unique_ptr<Job> job,
                                    const std::vector<std::uint64_t> & args) = 0;
    virtual std::uint64_t push_back(std::unique_ptr<Result>) = 0;
    
    virtual void sync() = 0;
    virtual void sync(std::uint64_t) = 0;
    virtual void clear() = 0;
    virtual void clear(std::uint64_t) = 0;
    
    virtual const Result & operator[](std::uint64_t) const = 0;
    virtual Result & operator[](std::uint64_t) = 0;
    virtual std::unique_ptr<Result> hand_over(std::uint64_t) = 0;
    
    virtual inline ~Graph_executor();
};

inline Graph_executor::~Graph_executor()
{}

#endif
