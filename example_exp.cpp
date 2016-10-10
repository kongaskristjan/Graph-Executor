
#include <example_exp.hpp>

Example_exp::Example_exp(uint64_t _base, uint64_t _pow):
    base(_base), pow(_pow)
{}


std::unique_ptr<Example_job> Example_exp::clone() const
{
    return std::make_unique<Example_exp>(base, pow);
}


// Brute force implementation for consuming CPU
std::unique_ptr<Result> Example_exp::execute(
    const std::vector<const Result *> &) const
{
    uint64_t prod = 1;
    for (uint64_t i = 0; i < pow; ++i)
        prod *= base;

    return std::make_unique<Integer>(prod);
}
