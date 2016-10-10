
#include <tester.hpp>

void Tester::add_executor(std::unique_ptr<Graph_executor> exec)
{
    assert(! total); // No adding between two executions
    executors.emplace_back(std::move(exec));
}


uint64_t Tester::push(const Example_job & job, const std::vector<uint64_t> & args)
{
    std::vector<Graph_ptr *> exec_args(args.size());
    for (auto & exec: executors){
        for (size_t i = 0; i < args.size(); ++i)
            exec_args[i] = exec.result_map[args[i]].get();
        
        exec.result_map.push_back
            (exec.exec->push(-1, job.clone(), exec_args));
    }

    return total++;
}


uint64_t Tester::push(const Example_result & result)
{
    for (auto & exec: executors)
        exec.result_map.push_back
            (exec.exec->push(-1, result.clone()));

    return total++;
}


void Tester::verify_and_end_test()
{
    std::vector<uint64_t> access = gen_shuffled();
    
    for (uint64_t i: access){
        const Graph_ptr & ptr_0 = * executors[0].result_map[i];
        unsigned hash_0 = static_cast<const Example_result &>
            ((* executors[0].exec)[ptr_0]).hash();
        
        for (size_t j = 1; j < executors.size(); ++j){
            const Graph_ptr & ptr_i = * executors[0].result_map[i];
            unsigned hash_i = static_cast<const Example_result &>
                ((* executors[j].exec)[ptr_i]).hash();

            if (hash_0 != hash_i){
                std::cerr << "Graph_executors give different answers\n";
                std::terminate();
            }
        }
    }

    std::vector<uint64_t> hand_over = gen_shuffled();
    for (uint64_t i: hand_over)
        for (auto & exec: executors)
            exec.exec->hand_over(* exec.result_map[i]);

    total = 0;
    for (auto & exec: executors){
        exec.exec->clear();
        exec.result_map.clear();
    }
}


std::vector<uint64_t> Tester::gen_shuffled() const
{
    std::vector<uint64_t> v(total);
    std::iota(v.begin(), v.end(), 0);
    std::random_shuffle(v.begin(), v.end());
    return v;
}


Tester::Exec_holder::Exec_holder(std::unique_ptr<Graph_executor> _exec):
    exec(std::move(_exec))
{}
