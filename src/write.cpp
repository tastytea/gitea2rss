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
#include <cstdlib>
#include <iostream>
#include <regex>

namespace gitea2rss
{

using std::cout;

void write_line(const uint8_t spaces, const string &tag, const string &value)
{
    string endtag;
    // If there is a space in the tag, use only the part up until the space for
    // the ending tag.
    const size_t pos = tag.find(' ');
    if (pos == std::string::npos)
    {
        endtag = tag;
    }
    else
    {
        endtag = tag.substr(0, pos);
    }

    cout << std::string(spaces, ' ');
    cout << '<' << tag << '>' << value << "</" << endtag << ">\n";
}

void write_preamble(const string &url, const string &type)
{
    const string request_uri{get_env_var("REQUEST_URI")};
    const string server_name{get_env_var("SERVER_NAME")};
    const string https{get_env_var("HTTPS")};
    string selfurl;

    if (!request_uri.empty() && !server_name.empty())
    {
        if (https == "on")
        {
            selfurl = "https://";
        }
        else
        {
            selfurl = "http://";
        }
        selfurl += string(server_name) + request_uri;
        selfurl = std::regex_replace(selfurl, std::regex("&"), "&amp;");
    }

    cout << "<rss version=\"2.0\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n"
            "  <channel>\n";
    if (cgi)
    {
        cout << "    <atom:link href=\"" + selfurl
                    + "\" rel=\"self\" type=\"application/rss+xml\"/>\n";
    }

    write_line(4, "title", get_project(url) + " " + type);
    write_line(4, "link", url);
    write_line(4, "description", "List of " + type + " of " + get_repo(url));
    write_line(4, "generator", string("gitea2rss ") + version);
    write_line(4, "lastBuildDate", strtime(system_clock::now()));
}

} // namespace gitea2rss
