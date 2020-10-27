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
#include <iostream>
#include <json/json.h>
#include <sstream>

using std::cerr;
using std::cout;
using std::endl;
using std::stringstream;

namespace gitea2rss
{

uint8_t write_releases(const string &url)
{
    const string baseurl = get_baseurl(url);
    const string repo = get_repo(url);
    stringstream data(
        get_http(baseurl + "/api/v1/repos/" + repo + "/releases"));

    if (cgi)
    {
        cout << endl;
    }
    if (data.str().empty())
    {
        cerr << "Error: Could not download releases.\n";
        return 2;
    }

    write_preamble(url, "releases");

    Json::Value json;
    data >> json;

    for (const Json::Value &release : json)
    {
        const bool prerelease = release["prerelease"].asBool();
        const string type = (prerelease ? "Pre-Release" : "Stable");
        const string body = escape_some_html(release["body"].asString());

        cout << "    <item>\n";
        write_line(6, "title",
                   get_project(url) + ": " + release["name"].asString());
        write_line(6, "link",
                   ((baseurl + "/" += repo) += "/releases/tag/") +=
                   release["tag_name"].asString());
        write_line(6, "guid isPermaLink=\"false\"",
                   get_domain(url) + " release " + release["id"].asString());
        write_line(6, "pubDate", strtime(release["published_at"].asString()));
        write_line(6, "description",
                   "\n        <![CDATA[<p><strong>" + type
                       + "</strong></p>\n"
                         "<pre>"
                       + body
                       + "</pre>\n"
                         "        <p><a href=\""
                       + release["tarball_url"].asString()
                       + "\">Download tarball</a></p>]]>\n      ");
        cout << "    </item>\n";
    }

    return 0;
}

} // namespace gitea2rss
