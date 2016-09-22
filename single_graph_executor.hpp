#ifndef __SINGLE_GRAPH_EXECUTOR_HPP__
#define __SINGLE_GRAPH_EXECUTOR_HPP__

#include <graph_executor.hpp>
#include <job.hpp>
#include <limits>
#include <memory>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <deque>
#include <vector>
#include <iostream>

/*
  Single_graph_executor:
  
  All jobs are done using the main thread. Thus all jobs
  must be possible to do using a single thread. Some (complex
  and obscure) constructs are not.
*/

class Single_graph_executor: public Graph_executor {
public:
    uint64_t push(size_t dep_count, std::unique_ptr<Job> job,
                  const std::vector<uint64_t> & args) override;
    uint64_t push(size_t dep_count, std::unique_ptr<Result>) override;
    void clear() override;
    
    const Result & operator[](uint64_t) override;
    std::unique_ptr<Result> hand_over(uint64_t) override;
    std::unique_ptr<Result> force_hand_over(uint64_t) override;

    struct Task {
        Task() = default;
        Task(const Task &) = delete;
        Task(int64_t, std::unique_ptr<Job>, const std::vector<uint64_t> &);
        Task(int64_t, std::unique_ptr<Result>);

        int64_t dep_count = 0;
        std::unique_ptr<Job> job = nullptr;
        std::unique_ptr<Result> result = nullptr;
        std::vector<uint64_t> args;
    };
    
private:
    void calculate(uint64_t);
    void sync(uint64_t); // do every job before i
    int64_t correct_dep(int64_t); // -1  ->  max<int64_t>()

    inline void check_invariant();
    inline void good_arg(uint64_t);
    
    bool pushing = 0; /* This flag is always false, unless in push
                         function. Avoids a data race, when pushing
                         from a Task function. */
    
    uint64_t offset = 0;
    uint64_t synced = 0;
    uint64_t total = 0;
    std::deque<Task> tasks;
};


inline void Single_graph_executor::check_invariant()
{
    assert(! pushing);
    assert(offset <= total);
    assert(offset <= synced);
    assert(synced <= total);
    assert(offset + tasks.size() == total);
}


inline void Single_graph_executor::good_arg(uint64_t arg)
{
    (void) arg;
    
    assert(arg < total); // Argument exists
    assert(arg >= offset); // Argument is cleared
    assert(tasks[arg - offset].dep_count > 0); // Dep count is non-zero
    assert(tasks[arg - offset].result); // Result exists
}

#endif
