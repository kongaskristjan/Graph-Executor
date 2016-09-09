
#include <graph_executor.hpp>
#include <example_add.hpp>
#include <iostream>
#include <memory>
#include <vector>

void test_add()
{
    std::cout << "Testing addition\n";
    const int n = 5;
    std::vector<Result *> muls;
    for (int i = 0; i < n; ++i)
        muls.push_back(new Integer(i));
    
    std::unique_ptr<Job> add(new Add);
    std::unique_ptr<Result> res(add->execute(muls));
    std::cout << static_cast<Integer *>(res.get())->x << "\n";
    
    for (int i = 0; i < n; ++i)
        delete muls[i];
}


int main()
{
    test_add();
    return 0;
}
