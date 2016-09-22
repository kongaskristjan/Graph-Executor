#ifndef __TESTER_HPP__
#define __TESTER_HPP__

#include <graph_executor.hpp>
#include <example_job.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <iostream>
#include <exception>


// Compares different executors and veryfies the
// equality of results from different executors.
// Assumes deterministic befaviour of examples.

class Tester {
public:
    void add_executor(std::unique_ptr<Graph_executor>);

    // Tester's push() is guaranteed to return indeces 0, 1, ... in this order
    uint64_t push(const Example_job &, const std::vector<uint64_t> &);
    uint64_t push(const Example_result &);

    // Accesses and hands over all calculations, clears examples for next test.
    void verify_and_end_test();

    struct Exec_holder {
        std::unique_ptr<Graph_executor> exec;
        std::vector<uint64_t> args_map;

        Exec_holder() = default;
        Exec_holder(std::unique_ptr<Graph_executor>);
    };
    
private:
    std::vector<uint64_t> gen_shuffled() const;

    std::vector<Exec_holder> executors;
    uint64_t total = 0;
};

#endif
