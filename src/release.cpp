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
using std::endl;
using std::stringstream;

uint8_t releases(const string &url)
{
    const string baseurl = get_baseurl(url);
    const string repo = get_repo(url);

    stringstream data(get_http(baseurl + "/api/v1/repos/"
                               + repo + "/releases"));
    if (cgi)
    {
        cout << endl;
    }
    if (data.str().empty())
    {
        cerr << "Error: Could not download releases.\n";
        return 2;
    }

    Json::Value json;
    data >> json;

    for (const Json::Value &release : json)
    {
        const bool prerelease = release["prerelease"].asBool();
        const string type = (prerelease ? "Pre-Release" : "Stable");
        cout << "    <item>\n";
        write_line(6, "title", get_project(url) + ": " + release["name"].asString());
        write_line(6, "link", baseurl + "/" + repo + "/releases");
        write_line(6, "guid isPermaLink=\"false\"",
                   get_domain(url) + " release " + release["id"].asString());
        write_line(6, "pubDate", strtime(release["published_at"].asString()));
        write_line(6, "description",
                   "\n        <![CDATA[<p><strong>" + type + "</strong></p>\n"
                   "<pre>" + release["body"].asString() + "</pre>\n"
                   "        <p><a href=\"" + release["tarball_url"].asString()
                   + "\">Download tarball</a></p>" + "]]>\n      ");
        cout << "    </item>\n";
    }

    return 0;
}
