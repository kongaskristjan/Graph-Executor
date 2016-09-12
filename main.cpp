
#include <graph_executor.hpp>
#include <single_graph_executor.hpp>
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


void test_graph_executor(Graph_executor & executor)
{
    // Generate Fibonacci numbers
    const int n = 10;
    
    std::vector<uint64_t> indeces;
    indeces.push_back(executor.push(std::unique_ptr<Result>(new Integer(0))));
    indeces.push_back(executor.push(std::unique_ptr<Result>(new Integer(1))));
    for (int i = 2; i <= n; ++i)
        indeces.push_back(
            executor.push(std::unique_ptr<Job>(new Add),
                          std::vector<uint64_t>
                          ({indeces[i - 2], indeces[i - 1]})));

    for (uint64_t index: indeces){
        std::cout << "index: " << index << "\n";
        std::cout << "ans: " << static_cast<const Integer &>(executor[index]).x << "\n";
    }
}


int main()
{
    test_add();
    std::unique_ptr<Graph_executor> executor(new Single_graph_executor(false));
    test_graph_executor(* executor.get());
    
    return 0;
}
