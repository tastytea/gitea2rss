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
#include <catch.hpp>
#include <chrono>
#include <string>

using std::string;
using namespace gitea2rss;

SCENARIO("strtime() works as expected", "[time]")
{
    GIVEN("Time as time_point")
    {
        std::chrono::system_clock::time_point tp;
        const string time = strtime(tp);

        THEN("The time is returned in the correct format")
        {
            REQUIRE(time == "Thu, 01 Jan 1970 00:00:00 +0000");
        }
    }

    GIVEN("Time as ISO 8601 string")
    {
        const string time = strtime("1970-01-01T00:00:00+0000");

        THEN("The time is returned in the correct format")
        {
            REQUIRE(time == "Thu, 01 Jan 1970 00:00:00 +0000");
        }
    }
}
