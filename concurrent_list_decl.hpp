#ifndef __CONCURRENT_LIST_DECL_HPP__
#define __CONCURRENT_LIST_DECL_HPP__

#include <memory>
#include <atomic>

// A lock-free implementation of singly linked list
// All elements are popped from and pushed to front.

// This class implements basic functionality just
// for Lock_free_thread_pool:
//     push(...)
//     pop() If list is empty, nullptr is returned

template<typename T>
struct Concurrent_node {
    Concurrent_node(const T &, std::shared_ptr<Concurrent_node<T>>);
    
    T x;
    std::shared_ptr<Concurrent_node<T>> next; // Only access atomically
};


template<typename T>
class Concurrent_list {
public:
    class ptr;
    
    void push(const T &);
    ptr pop();

    class ptr {
        std::shared_ptr<Concurrent_node<T>> p;

    public:
        ptr(std::shared_ptr<Concurrent_node<T>> _p): p(_p) {}
        T & operator*() { return p->x; }
        T * operator->() { return & p->x; }
        operator bool() const { return (bool) p; }
    };
    
private:
    std::shared_ptr<Concurrent_node<T>> head; // Only access atomically
};

#endif
