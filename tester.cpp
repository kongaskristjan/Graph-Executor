
#include <tester.hpp>

void Tester::add_executor(std::unique_ptr<Graph_executor> exec)
{
    assert(! total); // No adding between two executions
    executors.emplace_back(std::move(exec));
}


uint64_t Tester::push(const Example_job & job, const std::vector<uint64_t> & args)
{
    std::vector<uint64_t> exec_args(args.size());
    for (auto & exec: executors){
        for (size_t i = 0; i < args.size(); ++i)
            exec_args[i] = exec.args_map[args[i]];
        
        exec.args_map.push_back
            (exec.exec->push(-1, job.clone(), args));
    }

    return total++;
}


uint64_t Tester::push(const Example_result & result)
{
    for (auto & exec: executors)
        exec.args_map.push_back
            (exec.exec->push(-1, result.clone()));

    return total++;
}


void Tester::verify_and_end_test()
{
    std::vector<uint64_t> access = gen_shuffled();
    for (uint64_t i: access){
        unsigned hash_0 = static_cast<const Example_result &>
            ((* executors[0].exec)[i]).hash();
        for (size_t j = 1; j < executors.size(); ++j){
            unsigned hash_i = static_cast<const Example_result &>
                ((* executors[j].exec)[i]).hash();
            assert(hash_0 == hash_i); // hash[0] == hash[i]
            if (hash_0 != hash_i){
                std::cerr << "Graph_executors give different answers\n";
                std::terminate();
            }
        }
    }

    std::vector<uint64_t> hand_over = gen_shuffled();
    for (uint64_t i: hand_over)
        for (auto & exec: executors)
            exec.exec->hand_over(i);

    total = 0;
    for (auto & exec: executors){
        exec.exec->clear();
        exec.args_map.clear();
    }
}


std::vector<uint64_t> Tester::gen_shuffled() const
{
    std::vector<uint64_t> vec(total);
    std::iota(vec.begin(), vec.end(), 0);
    return vec;
}


Tester::Exec_holder::Exec_holder(std::unique_ptr<Graph_executor> _exec):
    exec(std::move(_exec))
{}
