#ifndef __PERFORM_DATA_STRUCTURE_TESTS_DECL_HPP__
#define __PERFORM_DATA_STRUCTURE_TESTS_DECL_HPP__

#include <thread_pool.hpp>
#include <atomic>
#include <random>
#include <memory>

template<template <class T> class Cont, class T>
void perform_ds_test(Thread_pool &, Cont<T> &);


template<template <class T> class Cont, class T>
class Ds_tester: public Thread_pool_job {
public:
    Ds_tester<Cont, T>(Cont<T> &, std::atomic<int64_t> & sz_cnt, int seed);
    void execute() override;

private:
    Cont<T> & cont;
    std::atomic<int64_t> & sz_cnt;
    int seed;
};

#endif
