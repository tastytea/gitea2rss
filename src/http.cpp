/*  This file is part of gitea2rss.
 *  Copyright © 2019, 2020 tastytea <tastytea@tastytea.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gitea2rss.hpp"
#include "version.hpp"
#include <curl/curl.h>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::runtime_error;
using std::to_string;

string buffer_body;

string get_http(const string &url)
{
    try
    {
        curl_global_init(CURL_GLOBAL_ALL); // NOLINT(hicpp-signed-bitwise)
        CURL *connection{curl_easy_init()};
        if (connection == nullptr)
        {
            throw runtime_error{"Failed to initialize curl."};
        }

        char buffer_error[256];

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(connection, CURLOPT_ERRORBUFFER, buffer_error);

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(connection, CURLOPT_WRITEFUNCTION, writer_body);

        CURLcode code{curl_easy_setopt(connection, CURLOPT_FOLLOWLOCATION, 1L)};
        if (code != CURLE_OK)
        {
            throw runtime_error{"HTTP is not supported."};
        }
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(connection, CURLOPT_MAXREDIRS, 5L);

        code =
            curl_easy_setopt(connection, CURLOPT_USERAGENT,
                             (string("gitea2rss/") += global::version).c_str());
        if (code != CURLE_OK)
        {
            throw runtime_error{"Failed to set User-Agent."};
        }

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(connection, CURLOPT_HTTPGET, 1L);

        code = curl_easy_setopt(connection, CURLOPT_URL, url.c_str());
        if (code != CURLE_OK)
        {
            throw runtime_error{"Couldn't set URL: " + to_string(code)};
        }

        code = curl_easy_perform(connection);
        if (code != CURLE_OK)
        {
            throw runtime_error{"libcurl error: " + to_string(code)};
        }

        long http_status{0}; // NOLINT(google-runtime-int)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_getinfo(connection, CURLINFO_RESPONSE_CODE, &http_status);

        curl_easy_cleanup(connection);

        if (http_status == 200)
        {
            return buffer_body;
        }

        if (cgi)
        {
            cout << "Status: " << http_status << endl;
        }
        throw runtime_error{"HTTP error: " + to_string(http_status)};
    }
    catch (const runtime_error &e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    return "";
}

size_t writer_body(char *data, size_t size, size_t nmemb)
{
    if (data == nullptr)
    {
        return 0;
    }

    buffer_body.append(data, size * nmemb);

    return size * nmemb;
}
