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
#include "version.hpp"
#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::chrono::system_clock;

int main(int argc, char *argv[])
{

    const string query{get_env_var("QUERY_STRING")};
    string url;
    string type = "releases";

    if (!query.empty())
    {
        cout << query << " AAAAAAAAA\n";
        const string baseurl{get_env_var("GITEA2RSS_BASEURL")};
        if (baseurl.empty())
        {
            cout << "Status: 500 Internal Server Error\n\n";
            cerr << "Error: GITEA2RSS_BASEURL not set\n";
            return 1;
        }
        cgi = true;

        // Look for repo in QUERY_STRING.
        size_t pos_found = query.find("repo=");
        if (pos_found == std::string::npos)
        {
            cout << "Status: 400 Bad Request\n\n";
            return 1;
        }
        const size_t pos_repo = pos_found + 5;
        const size_t pos_endrepo = query.find('&', pos_repo) - pos_repo;
        url = string(baseurl) + "/" + query.substr(pos_repo, pos_endrepo);

        // Look for type in QUERY_STRING.
        pos_found = query.find("type=");
        if (pos_found != std::string::npos)
        {
            const size_t pos_type = pos_found + 5;
            type = query.substr(pos_type, query.find('&') - pos_type);
        }

        cout << "Content-Type: application/rss+xml\n";
    }
    else if (argc < 2)
    {
        cerr << "usage: " << argv[0]
             << " URL of Gitea project [releases|tags]\n";
        return 1;
    }
    else
    {
        url = argv[1];
        if (argc > 2)
        {
            type = argv[2];
        }
    }

    uint8_t ret = 0;
    if (type == "releases")
    {
        ret = write_releases(url);
    }
    else if (type == "tags")
    {
        ret = write_tags(url);
    }

    if (ret != 0)
    {
        return ret;
    }

    cout << "  </channel>\n"
            "</rss>\n";

    return 0;
}
