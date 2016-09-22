#ifndef __GRAPH_EXECUTOR_HPP__
#define __GRAPH_EXECUTOR_HPP__

#include <memory>
#include <cstdint>
#include <job.hpp>


/*
  Graph_executor:
  push() - push a job to be processed or
      a result to be used for initialization or to fill space
  
  clear() - release all results
  
  operator[] - return const reference to a result. In general
      avoid storing the reference/pointer, but if you must,
      don't clear before the reference is destroyed. If answer
      is not yet calculated, it will wait until it is calculated.
  hand_over() - hand result over to the main process. If answer is
      not yet calculated, it will wait until it is calculated.
      If answer is used by another job, Graph_executor waits until it's done.
  force_hand_over() - Same as hand_over, but doesn't wait if other
      jobs might use it according to dep_count.

  Graph_executor assumes at the moment, that no jobs are created within
      jobs themselves. However, this might change later.

  The following might be used as an expansion.
  """Graph_executor assumes that no threads are explicitly created by any
      jobs. Jobs may however be created by main thread or other jobs.
      However, those results may not be used (the only arguments used
      must be those given as arguments)."""
*/


class Graph_executor {
public:
    virtual uint64_t push(size_t dep_count, std::unique_ptr<Job>,
                          const std::vector<uint64_t> & args) = 0;
    virtual uint64_t push(size_t dep_count, std::unique_ptr<Result>) = 0;
    virtual void clear() = 0;
    
    virtual const Result & operator[](uint64_t) = 0;
    virtual std::unique_ptr<Result> hand_over(uint64_t) = 0;
    virtual std::unique_ptr<Result> force_hand_over(uint64_t) = 0;
    
    virtual inline ~Graph_executor();
};

inline Graph_executor::~Graph_executor()
{}

#endif
