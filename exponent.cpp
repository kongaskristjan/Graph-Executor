
#include <exponent.hpp>

Exponent::Exponent(uint64_t * _result, uint64_t _base, uint64_t _pow):
    result(_result), base(_base), pow(_pow)
{}


// Brute force implementation for consuming CPU
void Exponent::execute()
{
    uint64_t prod = 1;
    for (uint64_t i = 0; i < pow; ++i)
        prod *= base;

    * result = prod;
}
