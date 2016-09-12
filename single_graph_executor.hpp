#ifndef __SINGLE_GRAPH_EXECUTOR_HPP__
#define __SINGLE_GRAPH_EXECUTOR_HPP__

#include <graph_executor.hpp>
#include <job.hpp>
#include <memory>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <deque>
#include <vector>

/*
  Single_graph_executor:
  Async management is for compatibility and better debugging.
  if (async):   All execution is done right after push
  if (! async): excecution is done on accessing or syncing.

  This async definition is more similar to the usual one,
  where in async mode execution starts/continues at pushing, but
  is only done at syncing or accessing if this mode is off.
*/

class Single_graph_executor: public Graph_executor {
public:
    Single_graph_executor(bool _async = true);
    void set_async_mode(bool);
    bool async_mode() const;
    
    uint64_t push(std::unique_ptr<Job> job,
                  const std::vector<uint64_t> & args);
    uint64_t push(std::unique_ptr<Result>);
    
    void sync();
    void sync(uint64_t);
    void clear();
    void clear(uint64_t);
    
    const Result & operator[](uint64_t);
    std::unique_ptr<Result> hand_over(uint64_t);

    struct Task {
        Task() = default;
        Task(const Task &) = delete;
        Task(std::unique_ptr<Job>, const std::vector<uint64_t> &);
        Task(std::unique_ptr<Result>);

        std::unique_ptr<Job> job = nullptr;
        std::unique_ptr<Result> result = nullptr;
        std::vector<uint64_t> args;
        size_t dep_count = 0;
    };
    
private:
    void calculate(uint64_t);
    
    bool async;
    bool pushing = 0; /* This flag is always false, unless in push
                         function. Avoids recursive pushing/syncing
                         to avoid stack issues */
    
    uint64_t offset = 0;
    uint64_t synced = 0;
    uint64_t cleared = 0;
    uint64_t total = 0;
    std::deque<Task> tasks;
};

#endif
