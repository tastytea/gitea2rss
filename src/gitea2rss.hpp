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

#ifndef GITEA2RSS_HPP
#define GITEA2RSS_HPP

#include <chrono>
#include <cstdint>
#include <ctime>
#include <string>

namespace gitea2rss
{

using std::string;
using std::uint8_t;
using std::chrono::system_clock;

extern bool cgi;

//! Fetch HTTP document.
string get_http(const string &url);

// CURL receive function.
size_t writer_body(char *data, size_t size, size_t nmemb);

//! Convert time_point to RFC 822 compliant time string.
string strtime(const system_clock::time_point &timepoint);

//! Convert ISO 8601 time string to RFC 822 time string.
string strtime(const string &time);

//! Write line of XML.
void write_line(uint8_t spaces, const string &tag, const string &value);

//! Write the RSS preamble and channel data.
void write_preamble(const string &url, const string &type);

//! Write releases.
uint8_t write_releases(const string &url);

//! Write tags.
uint8_t write_tags(const string &url);

//! @brief Get the base URL, without trailing slash.
string get_baseurl(const string &url);

//! Get the domain name.
string get_domain(const string &url);

//! Get the full name of the repo (user/project).
string get_repo(const string &url);

//! Get the project name.
string get_project(const string &url);

//! Escape some characters to named HTML entities.
string escape_some_html(string html);

//! Return environment variable or "" if it is not set.
string get_env_var(const string &variable);

} // namespace gitea2rss

#endif // GITEA2RSS_HPP
