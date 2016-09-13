
#include <example_cpu.hpp>

Vector_int::Vector_int(size_t sz, bool nonzero): nums(sz)
{
    assert(sz);
    if (nonzero)
        for (size_t i = 0; i < sz; ++i)
            nums[i] = i;
}


bool operator==(const Vector_int & x, const Vector_int & y)
{
    return x.nums == y.nums;
}


std::ostream & operator<<(std::ostream & os, const Vector_int & v)
{
    os << "(" << v.nums[0];
    for (unsigned x: v.nums)
        os << ", " << x;
    os << ")";

    return os;
}


std::unique_ptr<Result> Cpu_consumer::execute(
    const std::vector<const Result *> & args) const
{
    assert(args.size() == 2);
    const Vector_int * arg0 = static_cast<const Vector_int *>(args[0]);
    const Vector_int * arg1 = static_cast<const Vector_int *>(args[1]);
    size_t sz = arg0->nums.size();

    // Does some pointless CPU time consuming
    // deterministic modular calculations
    std::unique_ptr<Vector_int> result(new Vector_int(sz));
    for (size_t i = 0; i < sz; ++i)
        for (size_t j = i; j < sz - 1; ++j)
            result->nums[i] += arg0->nums[j] ^ arg1->nums[j + 1];

    return std::move(result);
}
