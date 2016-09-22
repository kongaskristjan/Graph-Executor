
#include <example_cpu.hpp>

Vector_int::Vector_int(size_t sz, bool nonzero): nums(sz)
{
    assert(sz);
    if (nonzero)
        for (size_t i = 0; i < sz; ++i)
            nums[i] = i;
}


std::unique_ptr<Example_result> Vector_int::clone() const
{
    auto ret(std::make_unique<Vector_int>(nums.size()));
    ret->nums = nums;
    return ret;
}


unsigned Vector_int::hash() const
{
    unsigned h = 0;
    unsigned mul = 1;
    for (unsigned x: nums){
        h += mul * x;
        mul *= 83742932u;
    }

    return h;
}


std::ostream & operator<<(std::ostream & os, const Vector_int & v)
{
    os << "(" << v.nums[0];
    for (unsigned x: v.nums)
        os << ", " << x;
    os << ")";

    return os;
}


std::unique_ptr<Example_job> Cpu_consumer::clone() const
{
    return std::make_unique<Cpu_consumer>(); // Nothing to clone
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
