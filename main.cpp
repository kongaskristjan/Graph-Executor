
#include <thread_pool.hpp>
#include <graph_executor.hpp>
#include <single_graph_executor.hpp>
#include <tester.hpp>
#include <perform_tests.hpp>
#include <thread_pool_tester.hpp>
#include <perform_thread_pool_tests.hpp>
#include <lock_thread_pool.hpp>
#include <lock_free_thread_pool.hpp>
#include <lock_graph_executor.hpp>
#include <concurrent_list.hpp>
#include <perform_data_structure_tests.hpp>
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
        Lock_free_thread_pool lftp1(1);
        Lock_free_thread_pool lftp2(2);
        Lock_free_thread_pool lftp3(3);
    }
    
    Thread_pool_tester tester;
    for (int i = 1; i <= 24; ++i)
        tester.add(std::make_unique<Lock_thread_pool>(i));
    for (int i = 1; i <= 24; ++i)
        tester.add(std::make_unique<Lock_free_thread_pool>(i));

    perform_all_tests(tester);
}


void test_data_structures()
{
    for (int threads = 1; threads <= 10; ++threads){
        Lock_thread_pool tp(threads);

        {
            Concurrent_list<int> lst;
            perform_ds_test(tp, lst);
        }
    }
}


int main()
{
    srand(time(NULL));
    
    test_data_structures();
    test_thread_pool();
    test_graph_executor();

    return 0;
}
