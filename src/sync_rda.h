#pragma once

//
// sync_rda.h - A collection of some useful utils for synchronization.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <algorithm>
#include <functional>
#include <thread>
#include <vector>

#include "platform_defs.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4505, "-Wunused-function")

namespace rda
{
    namespace sync
    {
        // divide up the work from [start,end) among num_threads threads, calling the 'op' function with start and end divided up
        void divide_work_over_range(const size_t start, const size_t end, const size_t num_threads, const std::function<void(const size_t, const size_t)> & op)
        {
            // if range is empty, or no threads specified, just return immediately
            if (end <= start || num_threads == 0)
                return;

            // vector to contain the threads
            std::vector<std::thread> thread_vector;

            // calculates the number of elements each thread should work on (never zero)
            const size_t span = std::max(static_cast<size_t>(1), (end - start) / num_threads);

            // start index for an individual thread
            size_t start_point = start;

            // end index for an individual thread
            size_t end_point = std::min(start_point + span, end);

            // iterate over number of threads to create
            for (size_t i = 0; i < num_threads; ++i)
            {
                // if there is a valid range to process
                if (start_point < end_point)
                {
                    // create the thread
                    thread_vector.emplace_back(std::thread(op, start_point, end_point));

                    // update the starting index for the next thread
                    start_point = end_point;

                    // update the ending index for the next thread (capped at 'end')
                    end_point = std::min(start_point + span + 1, end);
                }
            }

            // join all the threads
            for (auto& t : thread_vector)
                t.join();
        }

    } // namespace sync

} // namespace rda

POP_WARN_DISABLE
