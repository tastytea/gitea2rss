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
#include <sstream>
#include <cstdlib>
#include <curlpp/cURLpp.hpp>
#include <jsoncpp/json/json.h>
#include "version.hpp"
#include "gitea2rss.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;
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

    size_t pos_repo = url.find('/', 8) + 1;
    const string baseurl = url.substr(0, pos_repo - 1);
    const string domain = baseurl.substr(baseurl.rfind('/') + 1);
    const string repo = url.substr(pos_repo);
    const string project = repo.substr(repo.find('/') + 1);
    const string now = strtime(system_clock::now());
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

    cout <<
        "<rss version=\"2.0\">\n"
        "  <channel>\n"
        "    <title>" << project << " releases</title>\n"
        "    <link>" << url << "</link>\n"
        "    <description>Releases of " << repo << "</description>\n"
        "    <generator>gitea2rss " << global::version << "</generator>\n"
        "    <lastBuildDate>" << now << "</lastBuildDate>\n";

    for (const Json::Value &release : json)
    {
        const bool prerelease = release["prerelease"].asBool();
        const string type = (prerelease ? "Pre-Release" : "Stable");
        cout << "    <item>\n";
        write_line(6, "title", project + ": " + release["name"].asString());
        write_line(6, "link", baseurl + "/" + repo + "/releases");
        write_line(6, "guid isPermaLink=\"false\"",
                   domain + " release " + release["id"].asString());
        write_line(6, "pubDate", strtime(release["published_at"].asString()));
        write_line(6, "description",
                   "\n        <![CDATA[<p><strong>" + type + "</strong></p>\n"
                   "<pre>" + release["body"].asString() + "</pre>\n"
                   "        <p><a href=\"" + release["tarball_url"].asString()
                   + "\">Download tarball</a></p>" + "]]>\n      ");
        cout << "    </item>\n";
    }

    cout <<
        "  </channel>\n"
        "</rss>\n";

    curlpp::terminate();

    return 0;
}
