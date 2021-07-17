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

            bool isInAutomaticPrivateRange() const
            {
                return a() == 169 && b() == 254;
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
                return !isZero() && !isInAutomaticPrivateRange() && !isInPrivateRange();
            }

        private:
            uint32_t m_ip = 0;

        }; // class ipv4_address

	} // namespace ipv4_util

} // namespace rda
