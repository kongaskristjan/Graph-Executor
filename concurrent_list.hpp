#ifndef __CONCURRENT_LIST_HPP__
#define __CONCURRENT_LIST_HPP__

#include <concurrent_list_decl.hpp>

// A lock-free implementation of singly linked list
// All elements are popped from and pushed to front.

// This class implements basic functionality just for
// Lock_free_thread_pool

template<typename T>
void Concurrent_list<T>::push(const T &)
{}


template<typename T>
std::shared_ptr<T> Concurrent_list<T>::pop()
{
    return std::shared_ptr<T>();
}

#endif
