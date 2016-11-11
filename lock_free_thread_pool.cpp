
#include <lock_free_thread_pool.hpp>

Lock_free_thread_pool::Lock_free_thread_pool(uint64_t nthreads)
{
    if (nthreads == 0)
        nthreads = std::thread::hardware_concurrency();

    for (uint64_t i = 0; i < nthreads; ++i)
        threads.emplace_back([=] { thread_wait(); });

    check_invariant();
}


Lock_free_thread_pool::~Lock_free_thread_pool()
{
    {
        std::unique_lock<std::shared_timed_mutex> lck(mtx);
        exit_flag = true;
        empty_cv.notify_all();
    }
    
    for (auto & th: threads)
        th.join();

    assert(! jobs.pop());
    check_invariant();
}


void Lock_free_thread_pool::push(std::unique_ptr<Thread_pool_job> x)
{
    {
        std::shared_lock<std::shared_timed_mutex> lck(mtx);
        ++undone_jobs;
        jobs.push(std::move(x));
    }

    empty_cv.notify_one();
    check_invariant();
}


void Lock_free_thread_pool::finish()
{
    {
        std::unique_lock<std::shared_timed_mutex> lck(mtx);
        finish_flag = true;
        empty_cv.notify_all();
        while (undone_jobs)
            finish_cv.wait(mtx);
        finish_flag = false;
    }

    check_invariant();
}


void Lock_free_thread_pool::thread_wait()
{
    while (true){
        auto x = get_job();
        
        if (x){
            (* x)->execute();
            --undone_jobs;
        }

        else if (finish_flag){
            if (undone_jobs == 0)
                finish_cv.notify_all();
        }

        else if (exit_flag)
            break;
    }
}


Concurrent_list<std::unique_ptr<Thread_pool_job>>::ptr Lock_free_thread_pool::
get_job()
{
    auto x = jobs.pop();
    if (x)
        return x;
    
    {
        std::unique_lock<std::shared_timed_mutex> lck(mtx);
        while (true){
            x = jobs.pop();
            if (x)
                return x;
            
            if (finish_flag || exit_flag)
                return x; // x == nullptr
            
            empty_cv.wait(lck);
        }
    }

    std::terminate();
    return jobs.pop();
}
