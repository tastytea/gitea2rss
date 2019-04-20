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
#include <string>
#include <cstdlib>
#include <curlpp/cURLpp.hpp>
#include "version.hpp"
#include "gitea2rss.hpp"

using std::cout;
using std::cerr;
using std::string;
using std::chrono::system_clock;

bool cgi = false;

int main(int argc, char *argv[])
{
    const char *envquery = std::getenv("QUERY_STRING");
    string url;

    curlpp::initialize();

    if (envquery != nullptr)
    {
        const string query = envquery;
        const char *envbaseurl = std::getenv("GITEA2RSS_BASEURL");
        if (envbaseurl == nullptr)
        {
            cout << "Status: 500 Internal Server Error\n\n";
            cerr << "Error: GITEA2RSS_BASEURL not set\n";
            return 1;
        }
        cgi = true;

        const size_t pos = query.find("repo=");
        if (pos == std::string::npos)
        {
            cout << "Status: 400 Bad Request\n\n";
            return 1;
        }

        url = string(envbaseurl) + "/" + query.substr(query.find('=', pos) + 1);
        cout << "Content-Type: application/rss+xml\n";
    }
    else if (argc < 2)
    {
        cerr << "usage: " << argv[0] << " URL of Gitea project\n";
        return 1;
    }
    else
    {
        url = argv[1];
    }

    const string now = strtime(system_clock::now());

    cout <<
        "<rss version=\"2.0\">\n"
        "  <channel>\n"
        "    <title>" << get_project(url) << " releases</title>\n"
        "    <link>" << url << "</link>\n"
        "    <description>Releases of " << get_repo(url) << "</description>\n"
        "    <generator>gitea2rss " << global::version << "</generator>\n"
        "    <lastBuildDate>" << now << "</lastBuildDate>\n";

    uint8_t ret = releases(url);
    if (ret != 0)
    {
        return ret;
    }

    cout <<
        "  </channel>\n"
        "</rss>\n";

    curlpp::terminate();

    return 0;
}
