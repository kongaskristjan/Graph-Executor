
#include <single_thread_pool.hpp>

void Single_thread_pool::push(std::unique_ptr<Thread_pool_job> job)
{
    jobs.push(std::move(job));
    
    if (! executing)
        execute();
    
    check_invariant();
}


void Single_thread_pool::wait()
{
    assert(! executing); // No wait from job's execute
    check_invariant();
}


void Single_thread_pool::execute()
{
    assert(! jobs.empty());
    assert(! executing);

    executing = true;
    jobs.front()->execute();
    executing = false;
    jobs.pop();
    
    check_invariant();
}
