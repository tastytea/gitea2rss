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
#include <string>

using std::string;
using namespace gitea2rss;

SCENARIO("get_http() works as expected", "[http]")
{
    WHEN("URL is valid")
    {
        const string answer = get_http("https://ip.tastytea.de/");

        THEN("Answer is not empty")
        {
            REQUIRE(!answer.empty());
        }
    }

    WHEN("URL is invalid")
    {
        const string answer = get_http("https://url.invalid/");

        THEN("Answer is empty")
        {
            REQUIRE(answer.empty());
        }
    }
}
