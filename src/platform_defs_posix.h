#pragma once

//
// platform_defs_posix.h - Useful platform-dependent macros and utilities for POSIX.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//
// 2020-05-08
//

#if defined(CURRENT_PLATFORM_POSIX)

#include <algorithm>
#include <exception>
#include <vector>
#include <sstream>
#include <string>
#include <vector>

namespace rda
{
    namespace platform_defs
    {
        class posix_error
        {
        public:
            std::string syscall_name;
            int error_num = 0;
            std::string error_name;
            std::string error_description;

            posix_error(const std::string &syscall_name_,
                        const int error_num_)
                : syscall_name(syscall_name_),
                  error_num(error_num_),
                  error_name(""),
                  error_description("Unknown error")
            {
            }

            posix_error(const std::string &syscall_name_,
                        const int error_num_,
                        const std::string &error_name_,
                        const std::string &error_description_)
                : syscall_name(syscall_name_),
                  error_num(error_num_),
                  error_name(error_name_),
                  error_description(error_description_)
            {
            }

            bool operator==(const posix_error &rhs) const
            {
                return (syscall_name == rhs.syscall_name &&
                        error_num == rhs.error_num);
            }
        }; // class posix_error

        const static std::vector<posix_error> POSIX_ERROR_LIST =
            {
                posix_error("close", EBADF, "EBADF", "fd isn't a valid open file descriptor"),
                posix_error("close", EINTR, "EINTR", "The close() call was interrupted by a signal"),
                posix_error("close", EIO, "EIO", "An I/O error occurred"),
                posix_error("close", ENOSPC, "ENOSPC", "No space left on device"),
                posix_error("close", EDQUOT, "EDQUOT", "Quota exceeded"),

                posix_error("listen", EACCES, "EACCES", "Permission to create a socket of the specified type and/or protocol is denied"),
                posix_error("listen", EAFNOSUPPORT, "EAFNOSUPPORT", "The implementation does not support the specified address family"),
                posix_error("listen", EINVAL, "EINVAL", "Unknown protocol, or protocol family not available"),
                posix_error("listen", EMFILE, "EMFILE", "The per-process limit on the number of open file descriptors has been reached"),
                posix_error("listen", ENFILE, "ENFILE", "The system-wide limit on the total number of open files has been reached"),
                posix_error("listen", ENOBUFS, "ENOBUFS", "The system-wide limit on the total number of open files has been reached"),
                posix_error("listen", ENOMEM, "ENOMEM", "The system-wide limit on the total number of open files has been reached"),
                posix_error("listen", EPROTONOSUPPORT, "EPROTONOSUPPORT", "The protocol type or the specified protocol is not supported within this domain"),

                posix_error("setsockopt", EBADF, "EBADF", "The argument sockfd is not a valid file descriptor"),
                posix_error("setsockopt", EFAULT, "EFAULT", "The address pointed to by optval is not in a valid part of the process address space"),
                posix_error("setsockopt", EINVAL, "EINVAL", "optlen invalid in setsockopt()"),
                posix_error("setsockopt", ENOPROTOOPT, "ENOPROTOOPT", "The option is unknown at the level indicated"),
                posix_error("setsockopt", ENOTSOCK, "ENOTSOCK", "The file descriptor sockfd does not refer to a socket"),

                posix_error("bind", EACCES, "EACCES", "The address is protected, and the user is not the superuser"),
                posix_error("bind", EADDRINUSE, "EADDRINUSE", "The port number was specified as zero in the socket address structure, but, upon attempting to bind to an ephemeral port, it was determined that all port numbers in the ephemeral port range are currently in use"),
                posix_error("bind", EBADF, "EBADF", "sockfd is not a valid file descriptor"),
                posix_error("bind", EINVAL, "EINVAL", "The socket is already bound to an address"),
                posix_error("bind", ENOTSOCK, "ENOTSOCK", "The file descriptor sockfd does not refer to a socket"),
                posix_error("bind", EADDRNOTAVAIL, "EADDRNOTAVAIL", "A nonexistent interface was requested or the requested address was not local"),
                posix_error("bind", EFAULT, "EFAULT", "addr points outside the user's accessible address space"),
                posix_error("bind", ELOOP, "ELOOP", "Too many symbolic links were encountered in resolving addr"),
                posix_error("bind", ENAMETOOLONG, "ENAMETOOLONG", "addr is too long"),
                posix_error("bind", ENOENT, "ENOENT", "A component in the directory prefix of the socket pathname does not exist"),
                posix_error("bind", ENOMEM, "ENOMEM", "Insufficient kernel memory was available"),
                posix_error("bind", ENOTDIR, "ENOTDIR", "A component of the path prefix is not a directory"),
                posix_error("bind", EROFS, "EROFS", "The socket inode would reside on a read-only filesystem"),

                posix_error("listen", EADDRINUSE, "EADDRINUSE", "Another socket is already listening on the same port"),
                posix_error("listen", EBADF, "EBADF", "The argument sockfd is not a valid file descriptor"),
                posix_error("listen", ENOTSOCK, "ENOTSOCK", "The file descriptor sockfd does not refer to a socket"),
                posix_error("listen", EOPNOTSUPP, "EOPNOTSUPP", "The file descriptor sockfd does not refer to a socket")

        }; // POSIX_ERROR_LIST

        class posix_exception : public std::exception
        {
        public:
            posix_error err;

            posix_exception(const std::string &syscall_name, const int error_num)
                : err(posix_error(syscall_name, error_num))
            {
                auto iter = std::find(POSIX_ERROR_LIST.begin(), POSIX_ERROR_LIST.end(), err);

                if (iter != POSIX_ERROR_LIST.end())
                    err = *iter;
            }

            virtual const char *what() const throw()
            {
                std::stringstream ss;
                ss << "Exception: " << err.syscall_name;
                ss << "() errno=" << err.error_num;
                ss << " [" << err.error_name << "] ";
                ss << err.error_description;
                return ss.str().c_str();
            }
        }; // class posix_exception

    } // namespace platform_defs
} // namespace rda

#endif
