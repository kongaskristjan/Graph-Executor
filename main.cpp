
#include <graph_executor.hpp>
#include <single_graph_executor.hpp>
#include <example_add.hpp>
#include <example_cpu.hpp>
#include <tester.hpp>
#include <perform_tests.hpp>
#include <iostream>
#include <memory>
#include <vector>

int main()
{
    Tester tester;
    tester.add_executor(std::make_unique<Single_graph_executor>());
    tester.add_executor(std::make_unique<Single_graph_executor>());

    for (int i = 0; i < 10; ++i)
        Perform_tests::all(tester);
    return 0;
}
