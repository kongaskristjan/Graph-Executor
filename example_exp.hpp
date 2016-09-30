#ifndef __EXAMPLE_EXP_HPP__
#define __EXAMPLE_EXP_HPP__

#include <example_job.hpp>
#include <example_add.hpp>
#include <memory>
#include <vector>
#include <cstdint>

class Example_exp: public Example_job {
public:
    Example_exp(uint64_t _base, uint64_t _pow);
    std::unique_ptr<Example_job> clone() const;
    std::unique_ptr<Result> execute(
        const std::vector<const Result *> &) const;

private:
    uint64_t base = 1;
    uint64_t pow = 0;
};

#endif
