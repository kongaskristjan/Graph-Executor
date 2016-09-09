#ifndef __JOB_HPP__
#define __JOB_HPP__

#include <memory>
#include <vector>

/*
  Result:
  Store information to derived classes of Result.
  Use dynamic_cast() to exctract this information.
*/

class Result {
    virtual inline ~Result() = 0;
};


class Job {
    virtual std::unique_ptr<Result> execute(std::vector<Result *>) = 0;
    virtual ~Job() = 0;
};


inline Result::~Result()
{}


inline Job::~Job()
{}

#endif
