#ifndef __EXAMPLE_ADD_HPP__
#define __EXAMPLE_ADD_HPP__

#include <example_job.hpp>
#include <string>
#include <memory>

// Modular arithmetic (% 2^32)
class Integer: public Example_result {
public:
    Integer() = default;
    Integer(unsigned);
    std::unique_ptr<Example_result> clone() const override;
    virtual unsigned hash() const override;

    unsigned x = 0;
};


class Add: public Example_job {
public:
    std::unique_ptr<Example_job> clone() const override;
    std::unique_ptr<Result> execute(
        const std::vector<const Result *> & args) const override;
};

#endif
