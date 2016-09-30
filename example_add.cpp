
#include <example_add.hpp>

Integer::Integer(unsigned _x): x(_x)
{}


std::unique_ptr<Example_result> Integer::clone() const
{
    return std::make_unique<Integer>(x);
}


unsigned Integer::hash() const
{
    return x;
}


std::unique_ptr<Example_job> Add::clone() const
{
    return std::make_unique<Add>();
}


std::unique_ptr<Result> Add::execute(
    const std::vector<const Result *> & args) const
{
    unsigned sum = 0; // Modular arithmetic (% 2^32)
    for (const Result * arg: args){
        const Integer * x = static_cast<const Integer *>(arg);
        sum += x->x;
    }
    
    return std::make_unique<Integer>(sum);
}
