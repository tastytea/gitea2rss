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

#include <string>
#include <catch.hpp>
#include "gitea2rss.hpp"

using std::string;

SCENARIO ("get_repo() works as expected", "[strings]")
{
    WHEN ("HTTPS URL is valid")
    {
        const string repo
            = get_repo("https://git.example.com/user/project");

        THEN ("The repo is correctly returned")
        {
            REQUIRE(repo == "user/project");
        }
    }
}
