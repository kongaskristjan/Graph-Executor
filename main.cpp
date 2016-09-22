
#include <graph_executor.hpp>
#include <single_graph_executor.hpp>
#include <example_add.hpp>
#include <example_cpu.hpp>
#include <iostream>
#include <memory>
#include <vector>

// Bad memory management
void test_add()
{
    std::cout << "Testing addition\n";
    const int n = 5;
    std::vector<const Result *> muls;
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
    indeces.push_back(executor.push(2, std::unique_ptr<Result>
                                    (new Vector_int(10, true))));
    indeces.push_back(executor.push(2, std::unique_ptr<Result>
                                    (new Vector_int(10, true))));
    
    std::unique_ptr<Example_job> ptr(new Cpu_consumer);
    std::cout << ptr->name() << "\n";
    
    for (int i = 2; i <= n; ++i){
        std::cerr << i << ": indeces: " << indeces[i - 2] << " " << indeces[i - 1] << "\n";
        indeces.push_back(
            executor.push(2, std::unique_ptr<Job>(new Cpu_consumer),
                          std::vector<uint64_t>
                          ({indeces[i - 2], indeces[i - 1]})));
    }

    for (uint64_t index: indeces){
        std::cout << "index: " << index << "\n";
        auto vec = static_cast<const Vector_int &>(executor[index]);
        std::cout << "ans: " << vec << " " << vec.hash() << "\n";
    }
}


int main()
{
    test_add();
    std::unique_ptr<Graph_executor> executor(new Single_graph_executor);
    test_graph_executor(* executor.get());
    
    return 0;
}
