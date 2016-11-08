#ifndef __CONCURRENT_LIST_HPP__
#define __CONCURRENT_LIST_HPP__

#include <concurrent_list_decl.hpp>

// A lock-free implementation of singly linked list
// All elements are popped from and pushed to front.

// This class implements basic functionality just for
// Lock_free_thread_pool

template<typename T>
Concurrent_node<T>::Concurrent_node(const T & _x,
                                    std::shared_ptr<Concurrent_node<T>> _next):
    x(_x),
    next(std::move(_next))
{}


// shared_ptr-s can be atomically swapped, no ref counts are invalidated
template<typename T>
void Concurrent_list<T>::push(const T & x)
{
    auto node = std::make_shared<Concurrent_node<T>>(x, std::atomic_load(& head));

    // ABA can't happen betwenn head and node->next,
    // as node->next is also a shared_ptr
    while (! std::atomic_compare_exchange_weak(& head, & (node->next), node))
        ;
}


template<typename T>
typename Concurrent_list<T>::ptr Concurrent_list<T>::pop()
{
    auto node = std::atomic_load(& head);

    // No ABA can occur, as node is kept alive while accessed
    while (node && ! std::atomic_compare_exchange_weak(& head, & node,
                                                       std::atomic_load(& (node->next))))
        ;

    return ptr(std::move(node));
}

#endif
