/*  This file is part of gitea2rss.
 *  Copyright © 2019 tastytea <tastytea@tastytea.de>
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

#include <string>
#include <chrono>
#include <ctime>
#include <cstdint>

using std::string;
using std::chrono::system_clock;
using std::uint8_t;

extern bool cgi;

//! Fetch HTTP document.
const string get_http(const string &url);

//! Convert time_point to RFC 822 compliant time string.
const string strtime(const system_clock::time_point &timepoint);

//! Convert ISO 8601 time string to RFC 822 time string.
const string strtime(const string &time);

//! Write line of XML.
void write_line(const uint8_t spaces, const string &tag, const string &value);

//! Write releases.
uint8_t write_releases(const string &url);

//! Write tags.
uint8_t write_tags(const string &url);

//! @brief Get the base URL, without trailing slash.
const string get_baseurl(const string &url);

//! Get the domain name.
const string get_domain(const string &url);

//! Get the full name of the repo (user/project).
const string get_repo(const string &url);

//! Get the project name.
const string get_project(const string &url);

#endif  // GITEA2RSS_HPP
