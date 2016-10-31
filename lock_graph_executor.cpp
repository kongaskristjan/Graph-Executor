
#include <lock_graph_executor.hpp>

Lock_graph_executor::Ge_job::Ge_job(const std::vector<Graph_ptr *> & _args,
                                    std::unique_ptr<Job> _job, size_t _nout):
    in_args(_args.size()),
    job(std::move(_job)),
    nout(_nout),
    sent_to_execution(false)
{
    for (size_t i = 0; i < in_args.size(); ++i)
        in_args[i] = Lock_graph_ptr(static_cast<Lock_graph_ptr *>(_args[i])->job_it);
}


Lock_graph_executor::Ge_job::Ge_job(std::unique_ptr<Result> _result,
                                    size_t _nout):
    result(std::move(_result)),
    nout(_nout),
    sent_to_execution(true)
{}


void Lock_graph_executor::Ge_job::init_in_lock(std::list<Lock_graph_executor
                                               ::Ge_job>::iterator it)
{
    for (Lock_graph_ptr arg: in_args){
        arg.job_it->out_args.push_back(Lock_graph_ptr(it));

        if (! arg.job_it->result)
            ++nin;
    }
}


Lock_graph_executor::Lock_graph_executor(std::unique_ptr<Thread_pool> _tp):
    tp(std::move(_tp))
{}


std::unique_ptr<Graph_ptr> Lock_graph_executor::
push(size_t nout, std::unique_ptr<Job> job,
     const std::vector<Graph_ptr *> & args)
{
    std::list<Ge_job> new_ge_job;
    new_ge_job.emplace_back(args, std::move(job), nout);
    auto ptr = std::make_unique<Lock_graph_ptr>(new_ge_job.begin());

    {
        std::lock_guard<std::mutex> lck(jobs_mtx);
        new_ge_job.begin()->init_in_lock(new_ge_job.begin());
        jobs.splice(jobs.begin(), new_ge_job);
    }

    try_calculate(* ptr.get());
    return std::move(ptr);
}


std::unique_ptr<Graph_ptr> Lock_graph_executor::
        push(size_t nout, std::unique_ptr<Result> result)
{
    std::list<Ge_job> new_ge_job;
    new_ge_job.emplace_back(std::move(result), nout);
    auto ptr = std::make_unique<Lock_graph_ptr>(new_ge_job.begin());

    {
        std::lock_guard<std::mutex> lck(jobs_mtx);
        jobs.splice(jobs.begin(), new_ge_job);
    }

    return std::move(ptr);
}


void Lock_graph_executor::clear()
{
    tp->finish();
    jobs.clear();
}


const Result & Lock_graph_executor::operator[](const Graph_ptr & ptr)
{
    auto lge_ptr = static_cast<const Lock_graph_ptr &>(ptr);
    sync(lge_ptr);

    assert(lge_ptr.job_it->result);
    return * (lge_ptr.job_it->result);
}


std::unique_ptr<Result> Lock_graph_executor::hand_over(const Graph_ptr & ptr)
{
    auto lge_ptr = static_cast<const Lock_graph_ptr &>(ptr);
    sync(lge_ptr);
    
    return std::move(lge_ptr.job_it->result);
}


void Lock_graph_executor::try_calculate(Lock_graph_ptr ptr)
{
    std::unique_ptr<Job> job;
    {
        std::lock_guard<std::mutex> lck(jobs_mtx);
        auto & ge_job = *ptr.job_it;
        
        if (ge_job.nin > 0 || ge_job.sent_to_execution)
            return;

        assert(ge_job.nin == 0);
        ge_job.sent_to_execution = true;
    }
    
    tp->push(std::make_unique<Executor_to_thread_pool>(* this, ptr.job_it));
}


void Lock_graph_executor::sync(Lock_graph_ptr)
{
    tp->finish();
}


Lock_graph_ptr::Lock_graph_ptr(std::list<Lock_graph_executor::Ge_job>::iterator
                               _job_it):
    job_it(_job_it)
{}


Executor_to_thread_pool::Executor_to_thread_pool(Lock_graph_executor & _ge,
                                                 std::list<Lock_graph_executor::Ge_job>::iterator _job_it):
    ge(_ge),
    job_it(_job_it)
{}


void Executor_to_thread_pool::execute()
{
    std::vector<const Result *> args(job_it->in_args.size());
    for (size_t i = 0; i < args.size(); ++i)
        args[i] = job_it->in_args[i].job_it->result.get();
    
    auto result = job_it->job->execute(args);

    std::list<Lock_graph_executor::Ge_job> trash_jobs;
    thread_local std::vector<std::list<Lock_graph_executor::Ge_job>::iterator>
        launchable(32);
    launchable.clear();
    
    {
        std::lock_guard<std::mutex> lck(ge.jobs_mtx);
        job_it->result = std::move(result);
        
        for (auto arg_ptr: job_it->in_args)
            if (--arg_ptr.job_it->nout == 0)
                trash_jobs.splice(arg_ptr.job_it, ge.jobs);
        
        for (auto arg_ptr: job_it->out_args)
            if (--arg_ptr.job_it->nin == 0)
                launchable.push_back(arg_ptr.job_it);
    }
    
    for (auto it: launchable)
        ge.try_calculate(Lock_graph_ptr(it));
}
