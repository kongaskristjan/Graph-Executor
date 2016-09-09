#ifndef __EXAMPLE_JOB_HPP__
#define __EXAMPLE_JOB_HPP__

#include <job.hpp>

// Modular arithmetic (% 2^32)
class Integer: public Result {
public:
    Integer();
    Integer(unsigned);
    
    unsigned x;
};


class Add: public Job {
public:
    std::unique_ptr<Result> execute(std::vector<Result *> args) override;
};

#endif
