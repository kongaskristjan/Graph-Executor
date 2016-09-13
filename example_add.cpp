
#include <example_add.hpp>

Integer::Integer()
{}


Integer::Integer(unsigned _x): x(_x)
{}


unsigned Integer::hash() const
{
    return x;
}


std::string Add::name() const
{
    return "Add";
}


std::unique_ptr<Result> Add::execute(
    const std::vector<const Result *> & args) const
{
    unsigned sum = 0; // Modular arithmetic (% 2^32)
    for (const Result * arg: args){
        const Integer * x = static_cast<const Integer *>(arg);
        sum += x->x;
    }
    
    return std::unique_ptr<Result>(new Integer(sum));
}
