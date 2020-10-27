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
#include <cstdlib>
#include <map>
#include <regex>

namespace gitea2rss
{

using std::getenv;

string get_baseurl(const string &url)
{
    const size_t pos = url.find('/', 8);
    return url.substr(0, pos);
}

string get_domain(const string &url)
{
    const string baseurl = get_baseurl(url);
    return baseurl.substr(baseurl.rfind('/') + 1);
}

string get_repo(const string &url)
{
    const size_t pos = url.find('/', 8) + 1;
    return url.substr(pos);
}

string get_project(const string &url)
{
    const string repo = get_repo(url);
    return repo.substr(repo.find('/') + 1);
}

string escape_some_html(string html)
{
    const std::map<const char, const string> names = {{'<', "&lt;"},
                                                      {'>', "&gt;"}};

    for (const auto &pair : names)
    {
        const std::regex re(string(1, pair.first));
        html = std::regex_replace(html, re, pair.second);
    }

    return html;
}

string get_env_var(const string &variable)
{
    const char *env{getenv(variable.c_str())};
    if (env != nullptr)
    {
        return env;
    }
    return "";
}

} // namespace gitea2rss
