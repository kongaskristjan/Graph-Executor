#ifndef __EXPONENT_HPP__
#define __EXPONENT_HPP__

#include <thread_pool_job.hpp>
#include <cstdint>

class Exponent: public Thread_pool_job {
public:
    Exponent(uint64_t * result, uint64_t base, uint64_t pow);
    void execute() override;

private:
    uint64_t * result = nullptr;
    uint64_t base = 1;
    uint64_t pow = 0;
};

#endif
