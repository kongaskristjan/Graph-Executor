#ifndef __EXAMPLE_JOB_HPP__
#define __EXAMPLE_JOB_HPP__

#include <job.hpp>
#include <string>
#include <memory>
#include <vector>

class Example_result: public Result {
public:
    virtual unsigned hash() const = 0;
    virtual ~Example_result();
};


class Example_job: public Job {
public:
    virtual std::string name() const = 0;
    virtual std::unique_ptr<Result> execute(
        const std::vector<const Result *> &) const = 0;
    virtual ~Example_job();
};


inline Example_result::~Example_result()
{}


inline Example_job::~Example_job()
{}

#endif
