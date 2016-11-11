#ifndef __PERFORM_DATA_STRUCTURE_TESTS_HPP__
#define __PERFORM_DATA_STRUCTURE_TESTS_HPP__

#include <perform_data_structure_tests_decl.hpp>

template<template <typename T> typename Cont, typename T>
void perform_ds_test(Thread_pool & tp, Cont<T> & cont)
{
    std::atomic<int64_t> sz_cnt{0};
    
    for (int i = 0; i < 200; ++i)
        tp.push(std::make_unique<Ds_tester<Cont, T>>(cont, sz_cnt, rand()));
    
    tp.finish();

    while (cont.pop())
        --sz_cnt;

    if (sz_cnt != 0){
        std::cerr << "Error\n";
        std::terminate(); // No elements have "vanished" spontaneously
    }
}


template<template <typename T> typename Cont, typename T>
Ds_tester<Cont, T>::Ds_tester(Cont<T> & _cont,
                              std::atomic<int64_t> & _sz_cnt,
                              int _seed):
    cont(_cont),
    sz_cnt(_sz_cnt),
    seed(_seed)
{}


template<template <typename T> typename Cont, typename T>
void Ds_tester<Cont, T>::execute()
{
    std::default_random_engine gen(seed);
    for (int i = 0; i < 200; ++i){
        if (gen() % 2){
            ++sz_cnt;
            cont.push(seed | 1);
        }
        else
            if (cont.pop())
                --sz_cnt;
    }
}

#endif
