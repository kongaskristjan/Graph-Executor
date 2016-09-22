#ifndef __PERFORM_TESTS_HPP__
#define __PERFORM_TESTS_HPP__

#include <tester.hpp>
#include <example_add.hpp>
#include <example_cpu.hpp>
#include <vector>
#include <random>
#include <chrono>

class Perform_tests {
public:
    static void all(Tester &, const size_t n = 10);
    
    // Assumes Tester has ended last test
    static void fibonacci(Tester &, const size_t n = 10);
    static void random_add(Tester &, const size_t n = 10, const size_t k = 2);
};

#endif
