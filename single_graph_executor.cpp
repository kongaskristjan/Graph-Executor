
#include <single_graph_executor.hpp>

Single_graph_executor::Task::Task(
    int64_t _dep_count,
    std::unique_ptr<Job> _job, const std::vector<uint64_t> & _args):
    dep_count(_dep_count), job(std::move(_job)), args(_args)
{}


Single_graph_executor::Task::Task(
    int64_t _dep_count, std::unique_ptr<Result> _result):
    dep_count(_dep_count), result(std::move(_result))
{}


uint64_t Single_graph_executor::push(
    size_t dep_count, std::unique_ptr<Job> job,
    const std::vector<uint64_t> & args)
{
    ++total;
    tasks.emplace_back(correct_dep(dep_count), std::move(job), args);
    for (const uint64_t a: tasks[total - offset - 1].args){
        good_arg(a);
        
        if (tasks[a - offset].dep_count == 0)
            tasks[a - offset].result.reset();
        else
            --tasks[a - offset].dep_count;
    }

    while (! pushing && synced < total){
        pushing = true;
        sync(total);
        pushing = false;
        // Pushing flag avoids recursion, thus eliminating
        // a data race and stack issues
    }

    check_invariant();
    return total - 1;
}


uint64_t Single_graph_executor::push(
    size_t dep_count, std::unique_ptr<Result> result)
{
    tasks.emplace_back(correct_dep(dep_count), std::move(result));
    ++total;
    check_invariant();
    return total - 1;
}


void Single_graph_executor::clear()
{
    sync(total);
    tasks.clear();
    offset = 0;
    synced = 0;
    total = 0;
    
    check_invariant();
}


const Result & Single_graph_executor::operator[](uint64_t index)
{
    assert(index >= offset); // Index has not been cleared
    assert(index < total); // Index exists
    sync(index + 1);
    
    assert(tasks[index].result); // Return is still owned by graph executor
    check_invariant();
    return * tasks[index].result;
}


std::unique_ptr<Result> Single_graph_executor::hand_over(uint64_t index)
{
    assert(index >= offset); // Index has not been cleared
    assert(index < total); // Index exists
    sync(index + 1);

    check_invariant();
    return std::move(tasks[index].result);
}


std::unique_ptr<Result> Single_graph_executor::force_hand_over(uint64_t index)
{
    tasks[index].dep_count = 0;
    return std::move(hand_over(index));
}


void Single_graph_executor::calculate(uint64_t index)
{
    if (tasks[index - offset].result)
        return;
    
    std::vector<const Result *> job_args;
    for (uint64_t a: tasks[index - offset].args){
        good_arg(a);
        job_args.push_back(tasks[a - offset].result.get());
    }
    
    tasks[index - offset].result
        = std::move(tasks[index - offset].job->execute(job_args));
}


void Single_graph_executor::sync(uint64_t index)
{
    assert(index <= total);
    for (uint64_t i = synced; i < index; ++i)
        calculate(i);
    synced = std::max(index, synced);
}


int64_t Single_graph_executor::correct_dep(int64_t dep)
{
    if (dep == -1)
        return std::numeric_limits<int64_t>::max();
    return dep;
}
