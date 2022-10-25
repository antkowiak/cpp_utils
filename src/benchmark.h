#pragma once

//
// benchmark.h - Simple utility to benchmark how long a task takes to complete.
//  Uses object lifetime.
//
// Written by Ryan Antkowiak
//

#include <chrono>
#include <iostream>
#include <string>

namespace rda
{
    class benchmark
    {
    private:
        std::string name;
        std::chrono::steady_clock::time_point start_time;

    public:
        benchmark() = delete;
        benchmark(const benchmark &) = delete;

        benchmark(const std::string &benchmark_name)
        {
            name = benchmark_name;
            start_time = std::chrono::steady_clock::now();
        }

        ~benchmark()
        {
            std::chrono::steady_clock::time_point end_time =
                std::chrono::steady_clock::now();

            std::cout << "Benchmark: " << name << ": "
                      << std::chrono::duration_cast<std::chrono::microseconds>(
                             end_time - start_time)
                             .count()
                      << " usec" << std::endl;
        }
    }; // class benchmark
} // namespace rda
