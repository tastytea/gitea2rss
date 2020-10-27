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
#include <iomanip>

namespace gitea2rss
{

string strtime(const system_clock::time_point &timepoint)
{
    constexpr uint16_t bufsize = 1024;
    std::time_t time = system_clock::to_time_t(timepoint);
    std::tm *tm{std::gmtime(&time)};
    char buffer[bufsize];
    std::strftime(buffer, bufsize, "%a, %d %b %Y %T %z", tm);
    return static_cast<const string>(buffer);
}

string strtime(const string &time)
{
    std::tm tm = {};
    tm.tm_isdst = -1; // Detect daylight saving time.
    std::stringstream ss(time);
    ss >> std::get_time(&tm, "%Y-%m-%dT%T"); // Assume time is UTC.
    return strtime(system_clock::from_time_t(timegm(&tm)));
}

} // namespace gitea2rss
