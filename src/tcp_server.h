#pragma once

//
// tcp_server.h - Simple TCP Server.
//
// Written by Ryan Antkowiak
//
// 2020-05-07
//

#include "platform_defs.h"

#if defined(CURRENT_PLATFORM_POSIX)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <cerrno>
#include <cstring>
#include <exception>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace rda
{
#if defined(CURRENT_PLATFORM_POSIX)
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
                   std::function<void(int)> accept_cb,
                   std::function<void(int, const std::vector<char> &)> recv_cb,
                   const int backlog_ = 16)
            : port(port_),
              accept_callback(std::move(accept_cb)),
              recv_callback(std::move(recv_cb)),
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
                if (::close(fd) == -1)
                    throw(platform_defs::posix_exception("close", errno));
        }

        void listen()
        {
            fd = ::socket(AF_INET, SOCK_STREAM, 0);

            if (fd == -1)
                throw(platform_defs::posix_exception("listen", errno));

            int optval = 1;

            const int setsockoptRetVal = ::setsockopt(fd,
                                                      SOL_SOCKET,
                                                      SO_REUSEADDR,
                                                      reinterpret_cast<const void *>(&optval),
                                                      sizeof(optval));

            if (setsockoptRetVal != 0)
                throw(platform_defs::posix_exception("setsockopt", errno));

            sockaddr_in socket_address;
            std::memset(&socket_address, 0, sizeof(socket_address));

            socket_address.sin_family = AF_INET;
            socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
            socket_address.sin_port = htons(port);

            const int bindRetVal = ::bind(fd,
                                          reinterpret_cast<const sockaddr *>(&socket_address),
                                          sizeof(socket_address));

            if (bindRetVal == -1)
                throw(platform_defs::posix_exception("bind", errno));

            const int listenRetVal = ::listen(fd, backlog);

            if (listenRetVal == -1)
                throw(platform_defs::posix_exception("listen", errno));

            // TODO keep a set of file descriptors to call select() on.
            // TODO handle new connections with accept()
        }

    }; // class tcp_server (posix)
#endif
} // namespace rda
