
#include <graph_executor.hpp>
#include <example_add.hpp>
#include <iostream>
#include <memory>
#include <vector>

void test_add()
{
    std::cout << "Testing addition\n";
    const int n = 5;
    const int loop = 1e6;
    std::vector<Result *> muls;
    for (int i = 0; i < n; ++i)
        muls.push_back((Result *) new Integer(i));

    for (int i = 0; i < loop; ++i){
        std::unique_ptr<Job> add((Job *) new Add);
        std::unique_ptr<Result> res(add->execute(muls));
    }

    for (int i = 0; i < n; ++i)
        delete muls[i];
}


int main()
{
    test_add();
    return 0;
}
