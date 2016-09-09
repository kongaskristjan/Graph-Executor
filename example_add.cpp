
#include <example_add.hpp>

Integer::Integer()
{}


Integer::Integer(unsigned _x): x(_x)
{}


std::unique_ptr<Result> Add::execute(std::vector<Result *> args)
{
    unsigned sum = 0; // Modular arithmetic (% 2^32)
    for (Result * arg: args){
        Integer * x = static_cast<Integer *>(arg);
        sum += x->x;
    }
    
    return std::unique_ptr<Result>(new Integer(sum));
}
