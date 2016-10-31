#ifndef __LOCK_GRAPH_EXECUTOR_HPP__
#define __LOCK_GRAPH_EXECUTOR_HPP__

#include <job.hpp>
#include <graph_executor.hpp>
#include <thread_pool.hpp>
#include <memory>
#include <cstdint>
#include <vector>
#include <list>
#include <mutex>
#include <iostream>
#include <cassert>

struct Lock_graph_ptr;
struct Executor_to_thread_pool;

class Lock_graph_executor: public Graph_executor {
public:
    Lock_graph_executor(std::unique_ptr<Thread_pool> _tp);
    
    std::unique_ptr<Graph_ptr> push(size_t nout, std::unique_ptr<Job>,
                                    const std::vector<Graph_ptr *> &) override;
    std::unique_ptr<Graph_ptr> push(size_t nout,
                                    std::unique_ptr<Result>) override;
    void clear() override;
    
    const Result & operator[](const Graph_ptr &) override;
    std::unique_ptr<Result> hand_over(const Graph_ptr &) override;

    struct Ge_job {
        std::vector<Lock_graph_ptr> in_args;
        std::vector<Lock_graph_ptr> out_args;
        std::unique_ptr<Job> job;
        std::unique_ptr<Result> result;
        
        // Number of inputs and outputs not yet satisfied
        // nin is calculated using in_args
        size_t nin = 0;
        size_t nout;
        bool sent_to_execution;

        Ge_job(const std::vector<Graph_ptr *> &,
               std::unique_ptr<Job>, size_t _nout);
        Ge_job(std::unique_ptr<Result>, size_t _nout);

        // Required for job (not result) constructor, called from lock
        void init_in_lock(std::list<Lock_graph_executor::Ge_job>::iterator);
    };

private:
    void try_calculate(Lock_graph_ptr);
    void sync(Lock_graph_ptr); // wait until ptr is calculated

    inline void check_invariant() const;
    
    std::list<Ge_job> jobs; // Jobs, which can't be started yet
    std::mutex jobs_mtx;
    std::unique_ptr<Thread_pool> tp;

    friend struct Executor_to_thread_pool;
};


inline void Lock_graph_executor::check_invariant() const
{}


struct Lock_graph_ptr: public Graph_ptr {
    std::list<Lock_graph_executor::Ge_job>::iterator job_it;

    Lock_graph_ptr() = default;
    Lock_graph_ptr(std::list<Lock_graph_executor::Ge_job>::iterator);
};


struct Executor_to_thread_pool: public Thread_pool_job {
    Lock_graph_executor & ge;
    std::list<Lock_graph_executor::Ge_job>::iterator job_it;

    Executor_to_thread_pool(Lock_graph_executor &, std::list<Lock_graph_executor::Ge_job>::iterator);
    void execute() override;
};

#endif
