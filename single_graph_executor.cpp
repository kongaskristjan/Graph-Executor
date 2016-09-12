
#include <single_graph_executor.hpp>

Single_graph_executor::Task::Task(
    std::unique_ptr<Job> _job, const std::vector<uint64_t> & _args):
    job(std::move(_job)), args(_args)
{}


Single_graph_executor::Task::Task(std::unique_ptr<Result> _result):
    result(std::move(_result))
{}


Single_graph_executor::Single_graph_executor(bool _async):
    async(_async)
{}


void Single_graph_executor::set_async_mode(bool _async)
{
    async = _async;
    if (async)
        sync();
}


bool Single_graph_executor::async_mode() const
{
    return async;
}


uint64_t Single_graph_executor::push(
    std::unique_ptr<Job> job,
    const std::vector<uint64_t> & args)
{
    tasks.emplace_back(std::move(job), args);
    for (const uint64_t arg: tasks[total - offset].args){
        assert(arg >= cleared); // Argument has not been marked as cleared
        assert(arg < total); // Argument does exist
        ++tasks[arg - offset].dep_count;
    }

    if (async)
        while (! pushing && synced < total){
            pushing = true;
            sync();
            pushing = false;
            // Pushing flag avoids recursion, thus eliminating
            // stack issues
        }
    
    return total++;
}


uint64_t Single_graph_executor::push(std::unique_ptr<Result> result)
{
    tasks.emplace_back(std::move(result));
    return total++;
}


void Single_graph_executor::sync()
{
    sync(total);
}


void Single_graph_executor::sync(uint64_t index)
{
    assert(index <= total);
    for (uint64_t i = synced; i < index; ++i)
        calculate(i);
    synced = std::max(index, synced);
}


void Single_graph_executor::clear()
{
    clear(total);
}


void Single_graph_executor::clear(uint64_t index)
{
    assert(index <= total);
    uint64_t last = std::min(index, offset + tasks.size() - 1);
    for (uint64_t i = cleared + 1; i <= last; ++i)
        if (! tasks[i - offset].dep_count)
            tasks[i - offset].result.reset();

    cleared = std::max(cleared, index);
    while (cleared > offset && ! tasks.empty() && ! tasks.front().dep_count){
        tasks.pop_front();
        ++offset;
    }
}


const Result & Single_graph_executor::operator[](uint64_t index)
{
    assert(index >= cleared); // Index has not been marked as cleared
    assert(index < offset + tasks.size()); // Index does exist
    sync(index + 1);
    
    assert(tasks[index].result); // Return is still owned by graph executor
    return * tasks[index].result;
}


std::unique_ptr<Result> Single_graph_executor::hand_over(uint64_t index)
{
    assert(index >= cleared); // Index has not been marked as cleared
    assert(index < offset + tasks.size()); // Index does exist
    
    uint64_t i = synced;
    while (! tasks[index].dep_count){
        assert(i < offset + tasks.size()); // Internal error
        sync(i + 1);
    }

    return std::move(tasks[index].result);
}


void Single_graph_executor::calculate(uint64_t index)
{
    if (tasks[index].result)
        return;
    
    thread_local std::vector<const Result *> job_args;
    job_args.clear();
    for (uint64_t arg: tasks[index].args)
        job_args.push_back(tasks[arg].result.get());
    tasks[index].result = std::move(tasks[index].job->execute(job_args));
}
