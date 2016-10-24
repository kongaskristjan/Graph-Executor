#ifndef __GRAPH_EXECUTOR_HPP__
#define __GRAPH_EXECUTOR_HPP__

#include <job.hpp>
#include <memory>
#include <cstdint>
#include <vector>

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

  Graph_executor assumes that no threads are explicitly created by any
      jobs. Jobs may however be created by main thread or other jobs.
      However, those results may not be used (the only arguments used
      must be those given as arguments).
*/

struct Graph_ptr {
};


class Graph_executor {
public:
    virtual std::unique_ptr<Graph_ptr> push(
        size_t dep_count, std::unique_ptr<Job>,
        const std::vector<Graph_ptr *> & args) = 0;
    virtual std::unique_ptr<Graph_ptr> push(
        size_t dep_count, std::unique_ptr<Result>) = 0;
    virtual void clear() = 0;
    
    virtual const Result & operator[](const Graph_ptr &) = 0;
    virtual std::unique_ptr<Result> hand_over(const Graph_ptr &) = 0;
    
    virtual inline ~Graph_executor();
};

inline Graph_executor::~Graph_executor()
{}

#endif
