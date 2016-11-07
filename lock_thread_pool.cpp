
#include <lock_thread_pool.hpp>

Lock_thread_pool::Lock_thread_pool(uint64_t nthreads)
{
    if (nthreads == 0)
        nthreads = std::thread::hardware_concurrency();
    
    for (uint64_t i = 0; i < nthreads; ++i)
        threads.emplace_back([=] { thread_wait(); });
}


Lock_thread_pool::~Lock_thread_pool()
{
    finish();
    
    exit_flag = true;
    worker_cv.notify_all();
    
    for (std::thread & thr: threads)
        thr.join();
}


void Lock_thread_pool::push(std::unique_ptr<Thread_pool_job> job)
{
    {
        std::lock_guard<std::mutex> jobs_lck(jobs_mtx);
        jobs.push_back(std::move(job));
        ++undone_jobs;
    }

    worker_cv.notify_one();
}


void Lock_thread_pool::finish()
{
    std::unique_lock<std::mutex> jobs_lck(jobs_mtx);
    finish_flag = true;

    worker_cv.notify_all();
    while (undone_jobs > 0)
        finish_cv.wait(jobs_lck);
    
    finish_flag = false;
    jobs_lck.unlock();

    check_invariant();
}


std::unique_ptr<Thread_pool_job> Lock_thread_pool::get_job()
{
    std::unique_ptr<Thread_pool_job> job;
    if (! jobs.empty()){
        job = std::move(jobs.back());
        jobs.pop_back();
    }

    return job;
}


void Lock_thread_pool::thread_wait()
{
    while (true) {
        std::unique_lock<std::mutex> jobs_lck(jobs_mtx);

        if (exit_flag)
            break;
        
        if (finish_flag && undone_jobs == 0)
            finish_cv.notify_one();

        if (jobs.empty())
            worker_cv.wait(jobs_lck);
        auto job = get_job();
        jobs_lck.unlock();

        if (job){
            job->execute();
            --undone_jobs;
        }
    }

    check_invariant();
}
