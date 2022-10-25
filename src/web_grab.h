#pragma once

//
// webgrab.h - Utility to wrap libcurl web requests.
//
// Written by Ryan Antkowiak
//
// 2020-10-24
//

#ifdef CURL_ROOT

#include <iostream>
#include <sstream>
#include <string>

#include "curl/curl.h"

namespace rda
{
    class web_grab
    {
    public:
        web_grab(const std::string& url)
            : m_url(url)
        {
        }

        size_t size() const
        {
            return response().size();
        }

        std::string response() const
        {
            return m_response.str();
        }

        std::string grab()
        {
            m_response.clear();

            if (auto curl = curl_easy_init())
            {
                curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &web_grab::write_callback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_response);
                curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                curl = nullptr;
            }

            return m_response.str();
        }

    private:
        static size_t write_callback(char* pData, size_t size, size_t nmemb, void* pUserData)
        {
            const size_t numChars = size * nmemb;

            if (auto pString = static_cast<std::stringstream*>(pUserData))
                (*pString) << static_cast<char*>(pData);

            return numChars;
        }

    private:
        std::string m_url;
        std::stringstream m_response;
    };
}

#endif // CURL_ROOT
