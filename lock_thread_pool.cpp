
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

    {
        std::lock_guard<std::mutex> lck(jobs_mtx);
        exit_flag = true;
    }
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
    bool decrease_undone_jobs = false;
    while (true) {
        std::unique_lock<std::mutex> jobs_lck(jobs_mtx);

	if(decrease_undone_jobs) {
	    --undone_jobs;
	    decrease_undone_jobs = false;
	}

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
	    decrease_undone_jobs = true;
        }
    }

    check_invariant();
}
