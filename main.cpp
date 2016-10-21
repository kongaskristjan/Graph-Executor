
#include <thread_pool.hpp>
#include <graph_executor.hpp>
#include <single_graph_executor.hpp>
#include <example_add.hpp>
#include <example_cpu.hpp>
#include <tester.hpp>
#include <perform_tests.hpp>
#include <thread_pool_tester.hpp>
#include <perform_thread_pool_tests.hpp>
#include <lock_thread_pool.hpp>
#include <iostream>
#include <memory>
#include <vector>

void test_graph_executor()
{
    Tester tester;
    tester.add_executor(std::make_unique<Single_graph_executor>());
    tester.add_executor(std::make_unique<Single_graph_executor>());

    for (int i = 0; i < 10; ++i)
        Perform_tests::all(tester);
}


void test_thread_pool()
{
    Thread_pool_tester tester;
    for (int i = 1; i <= 24; ++i)
        tester.add(std::make_unique<Lock_thread_pool>(i));

    perform_all_tests(tester);
}


int main()
{
    //    test_graph_executor();
    test_thread_pool();

    return 0;
}
