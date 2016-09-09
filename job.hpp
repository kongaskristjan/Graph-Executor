#ifndef __JOB_HPP__
#define __JOB_HPP__

#include <memory>
#include <vector>

/*
  Result:
  Store information to derived classes of Result.
  Use static_cast() to exctract this information.
*/

class Result {
public:
    virtual ~Result();
};


class Job {
public:
    virtual std::unique_ptr<Result> execute(std::vector<Result *>) = 0;
    virtual ~Job();
};


inline Result::~Result()
{}


inline Job::~Job()
{}

#endif
