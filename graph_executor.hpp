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
  
  push_job() - push a job to be processed
  sync(i) - force Graph_executor to finish jobs before i.
      Will not return before everything is done.
  clear(i) - release all results before i as soon as possible
      If i is not specified, sync or clear everything.
  
  operator[] - return result
  hand_over() - hand result over to main process
*/


class Graph_executor {
public:
    virtual void set_async_mode(bool) = 0;
    virtual bool async_mode() const = 0;
    
    virtual std::uint64_t push_job(std::unique_ptr<Job> job,
                                   const std::vector<std::uint64_t> & args) = 0;
    virtual void sync() = 0;
    virtual void sync(std::uint64_t) = 0;
    virtual void clear() = 0;
    virtual void clear(std::uint64_t) = 0;
    
    virtual const Result & operator[](std::uint64_t) const = 0;
    virtual Result & operator[](std::uint64_t) = 0;
    virtual std::unique_ptr<Result> hand_over(std::uint64_t) = 0;
    
    virtual inline ~Graph_executor() = 0;
};

inline Graph_executor::~Graph_executor()
{}

#endif
