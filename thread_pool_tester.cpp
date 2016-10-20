
#include <thread_pool_tester.hpp>

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

    uint64_t hash0 = 0;
    for (size_t i = 0; i < thread_pools.size(); ++i){
        auto before = std::chrono::steady_clock::now();
        uint64_t hashi = test(* thread_pools[i]);
        auto after = std::chrono::steady_clock::now();
        std::cout << "Test #" << i << ": " << std::chrono::duration_cast
            <std::chrono::milliseconds>(after - before).count() << "ms\n";
        
        if (i){
            if (hash0 != hashi){
                std::cerr << "Hashes for 0 (" << hash0 << ") and "
                          << i << "(" << hashi << ") don't match\n";
                std::terminate();
            }
        }
        else
            hash0 = hashi;

    }
}
