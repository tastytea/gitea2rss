/*  This file is part of gitea2rss.
 *  Copyright © 2021 tastytea <tastytea@tastytea.de>
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

#include <json/json.h>

#include <iostream>
#include <sstream>

namespace gitea2rss
{

using std::cerr;
using std::cout;
using std::endl;
using std::stringstream;

uint8_t write_commits(const string &url)
{
    const string baseurl = get_baseurl(url);
    const string repo = get_repo(url);
    stringstream data(get_http(baseurl + "/api/v1/repos/" + repo + "/commits"));

    if (cgi)
    {
        cout << endl;
    }

    if (data.str().empty())
    {
        cerr << "Error: Could not download commits.\n";
        return 2;
    }

    write_preamble(url, "commits");

    Json::Value json;
    data >> json;

    for (const Json::Value &commit : json)
    {
        const string sha = commit["sha"].asString();
        const string message = commit["commit"]["message"].asString();
        const string title = message.substr(0, message.find('\n'));
        const string commit_url = commit["html_url"].asString();
        const string author = commit["commit"]["author"]["email"].asString()
                              + " ("
                              + commit["commit"]["author"]["name"].asString()
                              + ")";
        const string date = strtime(
            commit["commit"]["author"]["date"].asString());

        cout << "    <item>\n";
        write_line(6, "title", get_project(url) + ": " + title);
        write_line(6, "link", commit_url);
        write_line(6, "guid isPermaLink=\"false\"",
                   get_domain(url) + " commit " + sha);
        write_line(6, "author", author);
        write_line(6, "pubDate", date);
        write_line(6, "description",
                   "\n        <![CDATA[<p>" + message + "</p>]]>\n      ");
        cout << "    </item>\n";
    }

    return 0;
}

} // namespace gitea2rss
