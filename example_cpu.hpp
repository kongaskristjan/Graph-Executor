#ifndef __EXAMPLE_CPU_HPP__
#define __EXAMPLE_CPU_HPP__

#include <example_job.hpp>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>

// For arithmetic modulo 2^32
class Vector_int: public Example_result {
public:
    Vector_int(size_t, bool nonzero = false);
    virtual unsigned hash() const override;

    std::vector<unsigned> nums;
};

std::ostream & operator<<(std::ostream &, const Vector_int &);

class Cpu_consumer: public Example_job {
public:
    std::string name() const override;
    std::unique_ptr<Result> execute(
        const std::vector<const Result *> &) const override;
};

#endif
