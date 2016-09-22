#ifndef __PERFORM_TESTS_HPP__
#define __PERFORM_TESTS_HPP__

#include <tester.hpp>
#include <example_add.hpp>
#include <example_cpu.hpp>

class Perform_tests {
public:
    static void all(Tester &);
    
    // Assumes Tester has ended last test
    static void fibonacci(Tester &, const int n = 10);
};

#endif
