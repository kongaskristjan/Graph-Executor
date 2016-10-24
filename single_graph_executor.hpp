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
    std::unique_ptr<Graph_ptr> push(
        size_t dep_count, std::unique_ptr<Job> job,
        const std::vector<Graph_ptr *> & args) override;
    std::unique_ptr<Graph_ptr> push(
        size_t dep_count, std::unique_ptr<Result>) override;
    void clear() override;
    
    const Result & operator[](const Graph_ptr &) override;
    std::unique_ptr<Result> hand_over(const Graph_ptr &) override;
    
    struct Single_graph_ptr: public Graph_ptr {
        uint64_t id;

        inline Single_graph_ptr() = default;
        inline Single_graph_ptr(uint64_t);
    };
    
    
    struct Task {
        Task() = default;
        Task(const Task &) = delete;
        Task(int64_t, std::unique_ptr<Job>, const std::vector<Graph_ptr *> &);
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

    inline uint64_t ptr_to_index(const Graph_ptr &);
    
    inline void check_invariant() const;
    inline void good_arg(uint64_t) const;
    
    bool pushing = 0; /* This flag is always false, unless in push
                         function. Avoids a data race, when pushing
                         from a Job's execute function. */
    
    uint64_t offset = 0;
    uint64_t synced = 0;
    uint64_t total = 0;
    std::deque<Task> tasks;
};


inline Single_graph_executor::Single_graph_ptr::Single_graph_ptr(uint64_t _id):
    id(_id)
{}


inline uint64_t Single_graph_executor::ptr_to_index(const Graph_ptr & ptr)
{
    return static_cast<const Single_graph_ptr &>(ptr).id;
}


inline void Single_graph_executor::check_invariant() const
{
    assert(! pushing);
    assert(offset <= total);
    assert(offset <= synced);
    assert(synced <= total);
    assert(offset + tasks.size() == total);
}


inline void Single_graph_executor::good_arg(uint64_t arg) const
{
    (void) arg; // otherwise compiler warns if in NDEBUG mode
    
    assert(arg < total); // Argument exists
    assert(arg >= offset); // Argument is cleared
    assert(tasks[arg - offset].dep_count > 0); // Dep count is non-zero
    assert(tasks[arg - offset].result); // Result exists
}

#endif
