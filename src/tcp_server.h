#pragma once

//
// tcp_server.h - Simple TCP Server.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//
// 2020-05-07
//

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#else
// TODO
#endif

#include <cstring>
#include <exception>
#include <functional>
#include <string>
#include <vector>

namespace rda
{
    class tcp_server_exception : public std::exception
    {
    public:
        const int error_code_number;
        const std::string error_code_str;
        const std::string message;

        tcp_server_exception(const int code_number,
                             const std::string &code_str,
                             const std::string &msg)
            : error_code_number(code_number),
              error_code_str(code_str),
              message(msg)
        {
        }
    };

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    class tcp_server
    {
    private:
        const int port;
        const std::function<void(int)> accept_callback;
        const std::function<void(int, const std::vector<char> &)> recv_callback;
        const int backlog;

        int fd = -1;

    public:
        tcp_server(const int port_,
                   const std::function<void(int)> &accept_cb,
                   const std::function<void(int, const std::vector<char> &)> &recv_cb,
                   const int backlog_ = 16)
            : port(port_),
              accept_callback(accept_cb),
              recv_callback(recv_cb),
              backlog(backlog_)
        {
        }

        ~tcp_server()
        {
            close_nothrow();
        }

        void close_nothrow()
        {
            if (fd != -1)
                ::close(fd);
            fd = -1;
        }

        void close()
        {
            if (fd != -1)
            {
                if (::close(fd) == -1)
                {
                    switch (errno)
                    {
                        case EBADF:
                            throw(tcp_server_exception(errno, "EBADF", "fd isn't a valid open file descriptor"));
                        case EINTR:
                            throw(tcp_server_exception(errno, "EINTR", "The close() call was interrupted by a signal"));
                        case EIO:
                            throw(tcp_server_exception(errno, "EIO", "An I/O error occurred"));
                        case ENOSPC:
                            throw(tcp_server_exception(errno, "ENOSPC", "ENOSPC"));
                        case EDQUOT:
                            throw(tcp_server_exception(errno, "EDQUOT", "EDQUOT"));
                        default:
                            throw(tcp_server_exception(errno, "", "Unknown error"));
                    }
                }
            }
        }

        void listen()
        {
            fd = ::socket(AF_INET, SOCK_STREAM, 0);

            if (fd == -1)
            {
                switch (errno)
                {
                    case EACCES:
                        throw(tcp_server_exception(errno, "EACCES", "Permission to create a socket of the specified type and/or protocol is denied"));
                    case EAFNOSUPPORT:
                        throw(tcp_server_exception(errno, "EAFNOSUPPORT", "The implementation does not support the specified address family"));
                    case EINVAL:
                        throw(tcp_server_exception(errno, "EINVAL", "Unknown protocol, or protocol family not available"));
                    case EMFILE:
                        throw(tcp_server_exception(errno, "EMFILE", "The per-process limit on the number of open file descriptors has been reached"));
                    case ENFILE:
                        throw(tcp_server_exception(errno, "ENFILE", "The system-wide limit on the total number of open files has been reached"));
                    case ENOBUFS:
                        throw(tcp_server_exception(errno, "ENOBUFS", "The system-wide limit on the total number of open files has been reached"));
                    case ENOMEM:
                        throw(tcp_server_exception(errno, "ENOMEM", "The system-wide limit on the total number of open files has been reached"));
                    case EPROTONOSUPPORT:
                        throw(tcp_server_exception(errno, "EPROTONOSUPPORT", "The protocol type or the specified protocol is not supported within this domain"));
                    default:
                        throw(tcp_server_exception(errno, "", "Unknown error"));
                }
            }

            int optval = 1;

            const int setsockoptRetVal = ::setsockopt(fd,
                                                      SOL_SOCKET,
                                                      SO_REUSEADDR,
                                                      reinterpret_cast<const void *>(&optval),
                                                      sizeof(optval));

            if (setsockoptRetVal != 0)
            {
                switch (errno)
                {
                    case EBADF:
                        throw(tcp_server_exception(errno, "EBADF", "The argument sockfd is not a valid file descriptor"));
                    case EFAULT:
                        throw(tcp_server_exception(errno, "EFAULT", "The  address  pointed  to  by optval is not in a valid part of the process address space"));
                    case EINVAL:
                        throw(tcp_server_exception(errno, "EINVAL", "optlen invalid in setsockopt()"));
                    case ENOPROTOOPT:
                        throw(tcp_server_exception(errno, "ENOPROTOOPT", "The option is unknown at the level indicated"));
                    case ENOTSOCK:
                        throw(tcp_server_exception(errno, "ENOTSOCK", "The file descriptor sockfd does not refer to a socket"));
                    default:
                        throw(tcp_server_exception(errno, "", "Unknown error"));
                }
            }

            sockaddr_in socket_address;
            std::memset(&socket_address, 0, sizeof(socket_address));

            socket_address.sin_family = AF_INET;
            socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
            socket_address.sin_port = htons(port);

            const int bindRetVal = ::bind(fd,
                                          reinterpret_cast<const sockaddr *>(&socket_address),
                                          sizeof(socket_address));

            if (bindRetVal == -1)
            {
                switch (errno)
                {
                    case EACCES:
                        throw(tcp_server_exception(errno, "EACCES", "The address is protected, and the user is not the superuser"));
                    case EADDRINUSE:
                        throw(tcp_server_exception(errno, "EADDRINUSE", "The port number was specified as zero in the socket address structure, but, upon attempting to bind to an ephemeral port, it was determined that all port numbers in the ephemeral port range are currently in use"));
                    case EBADF:
                        throw(tcp_server_exception(errno, "EBADF", "sockfd is not a valid file descriptor"));
                    case EINVAL:
                        throw(tcp_server_exception(errno, "EINVAL", "The socket is already bound to an address"));
                    case ENOTSOCK:
                        throw(tcp_server_exception(errno, "ENOTSOCK", "The file descriptor sockfd does not refer to a socket"));
                    case EADDRNOTAVAIL:
                        throw(tcp_server_exception(errno, "EADDRNOTAVAIL", "A nonexistent interface was requested or the requested address was not local"));
                    case EFAULT:
                        throw(tcp_server_exception(errno, "EFAULT", "addr points outside the user's accessible address space"));
                    case ELOOP:
                        throw(tcp_server_exception(errno, "ELOOP", "Too many symbolic links were encountered in resolving addr"));
                    case ENAMETOOLONG:
                        throw(tcp_server_exception(errno, "ENAMETOOLONG", "addr is too long"));
                    case ENOENT:
                        throw(tcp_server_exception(errno, "ENOENT", "A component in the directory prefix of the socket pathname does not exist"));
                    case ENOMEM:
                        throw(tcp_server_exception(errno, "ENOMEM", "Insufficient kernel memory was available"));
                    case ENOTDIR:
                        throw(tcp_server_exception(errno, "ENOTDIR", "A component of the path prefix is not a directory"));
                    case EROFS:
                        throw(tcp_server_exception(errno, "EROFS", "The socket inode would reside on a read-only filesystem"));
                    default:
                        throw(tcp_server_exception(errno, "", "Unknown error"));
                }
            }

            const int listenRetVal = ::listen(fd, backlog);

            if (listenRetVal == -1)
            {
                switch (errno)
                {
                    case EADDRINUSE:
                        throw(tcp_server_exception(errno, "EADDRINUSE", "Another socket is already listening on the same port"));
                    case EBADF:
                        throw(tcp_server_exception(errno, "EBADF", "The argument sockfd is not a valid file descriptor"));
                    case ENOTSOCK:
                        throw(tcp_server_exception(errno, "ENOTSOCK", "The file descriptor sockfd does not refer to a socket"));
                    case EOPNOTSUPP:
                        throw(tcp_server_exception(errno, "EOPNOTSUPP", "The file descriptor sockfd does not refer to a socket"));
                    default:
                        throw(tcp_server_exception(errno, "", "Unknown error"));
                }
            }

            // TODO keep a set of file descriptors to call select() on.
            // TODO handle new connections with accept()
        }

    }; // class tcp_server (posix)
#else
    class tcp_server
    {
    public:
        // TODO
    }; // class tcp_server (win32)
#endif

} // namespace rda
