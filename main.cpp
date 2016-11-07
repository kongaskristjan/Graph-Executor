
#include <thread_pool.hpp>
#include <graph_executor.hpp>
#include <single_graph_executor.hpp>
#include <tester.hpp>
#include <perform_tests.hpp>
#include <thread_pool_tester.hpp>
#include <perform_thread_pool_tests.hpp>
#include <lock_thread_pool.hpp>
#include <lock_graph_executor.hpp>
#include <iostream>

void test_graph_executor()
{
    Tester tester;
    tester.add_executor(std::make_unique<Single_graph_executor>());
    for (int i = 1; i < 20; ++i)
        tester.add_executor(std::make_unique<Lock_graph_executor>
                            (std::make_unique<Lock_thread_pool>(i)));

    for (int i = 0; i < 100; ++i)
        Perform_tests::all(tester);
}


void test_thread_pool()
{
    for (int i = 0; i < 3000; ++i){
        Lock_thread_pool ltp2(2);
        Lock_thread_pool ltp3(3);
    }
    
    Thread_pool_tester tester;
    for (int i = 1; i <= 24; ++i)
        tester.add(std::make_unique<Lock_thread_pool>(i));

    perform_all_tests(tester);
}


int main()
{
    test_thread_pool();
    test_graph_executor();

    return 0;
}
