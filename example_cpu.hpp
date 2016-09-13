#ifndef __EXAMPLE_CPU_HPP__
#define __EXAMPLE_CPU_HPP__

#include <job.hpp>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>

// For arithmetic modulo 2^32
class Vector_int: public Result {
public:
    std::vector<unsigned> nums;
    Vector_int(size_t, bool nonzero = false);
};

bool operator==(const Vector_int &, const Vector_int &);
std::ostream & operator<<(std::ostream &, const Vector_int &);

class Cpu_consumer: public Job {
public:
    std::unique_ptr<Result> execute(
        const std::vector<const Result *> &) const override;
};

#endif
