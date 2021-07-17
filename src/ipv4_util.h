#pragma once

//
// ipv4_util.h - Utilities for ipv4 addresses.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <bitset>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <sstream>
#include <string>
#include <iomanip>

namespace rda
{
	namespace ipv4_util
	{
        class ipv4_address
        {
        public:
            ipv4_address()
            {
                set(0);
            }

            ipv4_address(const std::string& ip)
            {
                set(ip);
            }

            ipv4_address(const uint32_t ip)
            {
                set(ip);
            }

            ipv4_address(const uint8_t an, const uint8_t bn, const uint8_t cn, const uint8_t dn)
            {
                set(an, bn, cn, dn);
            }

            std::string to_string() const
            {
                return std::to_string(a()) + "." +
                    std::to_string(b()) + "." +
                    std::to_string(c()) + "." +
                    std::to_string(d());
            }

            std::string to_bstring() const
            {
                std::string s;
                s += std::bitset<8>(a()).to_string() + " ";
                s += std::bitset<8>(b()).to_string() + " ";
                s += std::bitset<8>(c()).to_string() + " ";
                s += std::bitset<8>(d()).to_string();
                return s;
            }

            std::string to_hstring() const
            {
                std::stringstream ss;
                ss << std::hex << std::setfill('0')
                    << std::hex << std::setw(2) << static_cast<uint16_t>(a())
                    << std::hex << std::setw(2) << static_cast<uint16_t>(b())
                    << std::hex << std::setw(2) << static_cast<uint16_t>(c())
                    << std::hex << std::setw(2) << static_cast<uint16_t>(d());
                return ss.str();
            }

            void set(const std::string& ip)
            {
                uint8_t an = 0;
                uint8_t bn = 0;
                uint8_t cn = 0;
                uint8_t dn = 0;

#pragma push
#pragma warning(disable: 4996)
                if (std::sscanf(ip.c_str(), "%hhd.%hhd.%hhd.%hhd", &an, &bn, &cn, &dn) == 4)
                {
                    a(an);
                    b(bn);
                    c(cn);
                    d(dn);
                }
#pragma pop
            }

            void set(const uint32_t ip)
            {
                m_ip = ip;
            }

            void set(const uint8_t an, const uint8_t bn, const uint8_t cn, const uint8_t dn)
            {
                m_ip = 0;
                a(an);
                b(bn);
                c(cn);
                d(dn);
            }

            uint32_t get() const
            {
                return m_ip;
            }

            void a(const uint8_t n)
            {
                const uint32_t quad = n << 24;
                m_ip = (m_ip & 0x00ffffff) | quad;
            }

            void b(const uint8_t n)
            {
                const uint32_t quad = n << 16;
                m_ip = (m_ip & 0xff00ffff) | quad;
            }

            void c(const uint8_t n)
            {
                const uint32_t quad = n << 8;
                m_ip = (m_ip & 0xffff00ff) | quad;
            }

            void d(const uint8_t n)
            {
                const uint32_t quad = n << 0;
                m_ip = (m_ip & 0xffffff00) | quad;
            }

            uint8_t a() const
            {
                return (reinterpret_cast<const uint8_t*>(&m_ip))[3];
            }

            uint8_t b() const
            {
                return (reinterpret_cast<const uint8_t*>(&m_ip))[2];
            }

            uint8_t c() const
            {
                return (reinterpret_cast<const uint8_t*>(&m_ip))[1];
            }

            uint8_t d() const
            {
                return (reinterpret_cast<const uint8_t*>(&m_ip))[0];
            }

            bool isZero() const
            {
                return m_ip == 0;
            }

            bool isLocalhost() const
            {
                return a() == 127 && b() == 0 && c() == 0 && d() == 1;
            }

            bool isInLoopbackRange() const
            {
                return a() == 127;
            }

            bool isInLinkLocalRange() const
            {
                const uint8_t an = a();
                const uint8_t bn = a();

                return an == 169 && (bn == 254 || bn == 255);
            }

            bool isInPrivateRange() const
            {
                const uint8_t an = a();
                const uint8_t bn = a();

                if (an == 10)
                    return true;

                if (an == 192 && bn == 168)
                    return true;

                if (an == 172 && bn >= 16 && bn <= 31)
                    return true;

                return false;
            }

            bool isInPublicRange() const
            {
                return !isZero() && !isInLoopbackRange() && !isInLinkLocalRange() && !isInPrivateRange();
            }

        private:
            uint32_t m_ip = 0;

        }; // class ipv4_address

        struct subnet
        {
            uint8_t num_network_bits = 0;
            ipv4_address mask;
            uint32_t num_hosts = 0;

        }; // struct subnet

        const std::map<uint8_t, subnet> IPV4_SUBNETS
        {
            {8,  subnet {8,  ipv4_address(255,   0,   0,   0), 16777214} },
            {9,  subnet {9,  ipv4_address(255, 128,   0,   0),  8388606} },
            {10, subnet {10, ipv4_address(255, 192,   0,   0),  4194302} },
            {11, subnet {11, ipv4_address(255, 224,   0,   0),  2097150} },
            {12, subnet {12, ipv4_address(255, 240,   0,   0),  1048574} },
            {13, subnet {13, ipv4_address(255, 248,   0,   0),   524286} },
            {14, subnet {14, ipv4_address(255, 252,   0,   0),   262142} },
            {15, subnet {15, ipv4_address(255, 254,   0,   0),   131070} },
            {16, subnet {16, ipv4_address(255, 255,   0,   0),    65534} },
            {17, subnet {17, ipv4_address(255, 255, 128,   0),    32766} },
            {18, subnet {18, ipv4_address(255, 255, 192,   0),    16382} },
            {19, subnet {19, ipv4_address(255, 255, 224,   0),     8190} },
            {20, subnet {20, ipv4_address(255, 255, 240,   0),     4094} },
            {21, subnet {21, ipv4_address(255, 255, 248,   0),     2046} },
            {22, subnet {22, ipv4_address(255, 255, 252,   0),     1022} },
            {23, subnet {23, ipv4_address(255, 255, 254,   0),      510} },
            {24, subnet {24, ipv4_address(255, 255, 255,   0),      254} },
            {25, subnet {25, ipv4_address(255, 255, 255, 128),      126} },
            {26, subnet {26, ipv4_address(255, 255, 255, 192),       62} },
            {27, subnet {27, ipv4_address(255, 255, 255, 224),       30} },
            {28, subnet {28, ipv4_address(255, 255, 255, 240),       14} },
            {29, subnet {29, ipv4_address(255, 255, 255, 248),        6} },
            {30, subnet {30, ipv4_address(255, 255, 255, 252),        2} }

        }; // IPV4_SUBNETS

	} // namespace ipv4_util

} // namespace rda
