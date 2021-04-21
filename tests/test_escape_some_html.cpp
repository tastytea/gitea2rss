/*  This file is part of gitea2rss.
 *  Copyright © 2019-2021 tastytea <tastytea@tastytea.de>
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

SCENARIO("escape_some_html() works as expected", "[strings]")
{
    WHEN("String with escape-able characters")
    {
        const string escaped = escape_some_html("<p><small>.</small></p>");

        THEN("The HTML is escaped")
        {
            REQUIRE(escaped
                    == "&#x3C;p&#x3E;&#x3C;small&#x3E;.&#x3C;/"
                       "small&#x3E;&#x3C;/p&#x3E;");
        }
    }
}
