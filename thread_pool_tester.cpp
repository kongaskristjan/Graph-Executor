
#include <thread_pool_tester.hpp>
#include <unistd.h>

void Thread_pool_tester::add(std::unique_ptr<Thread_pool> tp)
{
    thread_pools.push_back(std::move(tp));
}


void Thread_pool_tester::test(uint64_t (* test)(Thread_pool &),
                              const std::string & name)
{
    std::cout << "Begin test: " << name << "\n";
    if (thread_pools.empty())
        return;

    uint64_t hash = 0;
    for (size_t i = 0; i < thread_pools.size(); ++i){
        auto before = std::chrono::steady_clock::now();
        uint64_t test_hash = test(* thread_pools[i]);
        sleep(1);
        thread_pools[i]->finish();
        auto after = std::chrono::steady_clock::now();
        std::cout << "Test #" << i << ": "
                  << std::chrono::duration_cast
            <std::chrono::microseconds>(after - before).count() << "us\n";
        
        if (i)
            assert(hash == test_hash);
        else
            hash = test_hash;
    }
}
