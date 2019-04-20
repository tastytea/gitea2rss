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

#include <iostream>
#include <sstream>
#include <jsoncpp/json/json.h>
#include "gitea2rss.hpp"

using std::cout;
using std::cerr;
using std::stringstream;

uint8_t write_tags(const string &url)
{
    const string baseurl = get_baseurl(url);
    const string repo = get_repo(url);
    stringstream data(get_http(baseurl + "/api/v1/repos/"
                               + repo + "/git/refs"));

    if (data.str().empty())
    {
        cerr << "Error: Could not download tags.\n";
        return 2;
    }

    Json::Value json;
    data >> json;

    for (const Json::Value &ref : json)
    {
        if (ref["object"]["type"].asString() != "tag")
        {
            continue;
        }

        const string name = ref["ref"].asString().substr(10);
        const string sha = ref["object"]["sha"].asString();

        cout << "    <item>\n";
        write_line(6, "title", get_project(url) + ": " + name);
        write_line(6, "link", baseurl + "/" + repo + "/src/tag/" + name);
        write_line(6, "guid isPermaLink=\"false\"",
                   get_domain(url) + " tag " + sha);
        write_line(6, "description",
                   "\n        <![CDATA[<p><strong>" + name + "</strong> "
                   "with checksum " + sha + "</p>\n"
                   "        <p><a href=\""
                   + baseurl + "/" + repo + "/archive/" + name + ".tar.gz"
                   + "\">Download tarball</a></p>]]>\n      ");

        cout << "    </item>\n";
    }

    return 0;
}
