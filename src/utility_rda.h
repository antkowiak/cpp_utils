#pragma once

//
// utility_rda.h - Misc utilities.
//
// Written by Ryan Antkowiak
//
// 2020-05-19
//

#include <cstdlib>
#include <utility>

namespace rda
{
    namespace utility
    {
        // wrapping RAII handling of allocating memory with malloc.
        template <typename T>
        class smart_malloc
        {
        public:
            // constructor
            smart_malloc(const size_t size)
            {
                if (size > 0)
                {
                    ptr = static_cast<T *>(std::malloc(size));

                    if (ptr != nullptr)
                        allocated_size = size;
                }
            }

            // destructor
            ~smart_malloc()
            {
                clear();
            }

            // returns pointer to the memory
            T *get() const
            {
                return ptr;
            }

            // returns true if contains a valid pointer
            bool is_valid() const
            {
                return ptr != nullptr;
            }

            // returns the size of memory that was allocated
            size_t size() const
            {
                return allocated_size;
            }

            // no default constructor
            smart_malloc() = delete;

            // no copy constructor
            smart_malloc(const smart_malloc &) = delete;

            // no assignment operator
            smart_malloc& operator = (const smart_malloc& rhs) = delete;

            // move constructor
            smart_malloc(smart_malloc&& rhs) noexcept
                : ptr(rhs.ptr), allocated_size(rhs.allocated_size)
            {
                rhs.ptr = nullptr;
                rhs.allocated_size = 0;
            }

            // move assignment operator
            smart_malloc& operator=(smart_malloc&& rhs) noexcept
            {
                ptr = rhs.ptr;
                allocated_size = rhs.allocated_size;

                rhs.ptr = nullptr;
                rhs.allocated_size = 0;
            }

        private:
            // the pointer to the allocated memory
            T *ptr = nullptr;

            // size of the buffer allocated
            size_t allocated_size = 0;

            // free the memory
            void clear()
            {
                if (ptr != nullptr)
                    std::free(ptr);

                ptr = nullptr;
                allocated_size = 0;
            }

        }; // class smart_malloc

    } // namespace utility

} // namespace rda
