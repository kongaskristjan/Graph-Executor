#ifndef __CONCURRENT_LIST_DECL_HPP__
#define __CONCURRENT_LIST_DECL_HPP__

#include <memory>
#include <atomic>

// A lock-free implementation of singly linked list
// All elements are popped from and pushed to front.

// This class implements basic functionality just
// for Lock_free_thread_pool:
//     push(...)
//     pop() If list is empty, null_ptr is returned

template<typename T>
class Concurrent_list {
public:
    Concurrent_list<T>() = default;
    
    void push(const T &);
    std::shared_ptr<T> pop();

private:
    std::shared_ptr<T> head;
};

#endif
