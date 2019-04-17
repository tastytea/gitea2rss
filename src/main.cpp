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
#include <exception>
#include <cstdint>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>
#include <jsoncpp/json/json.h>
#include "version.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::ostringstream;
using std::stringstream;
using std::uint16_t;
using std::chrono::system_clock;

namespace curlopts = curlpp::options;

// Fetch HTTP document.
const string get_http(const string &url)
{
    string answer;

    try
    {
        ostringstream oss;
        curlpp::Easy request;
        request.setOpt<curlopts::Url>(url);
        request.setOpt<curlopts::UserAgent>(string("gitea2rss/")
                                            + global::version);
        request.setOpt<curlopts::HttpHeader>({ "Connection: close" });
        request.setOpt<curlopts::FollowLocation>(true);
        request.setOpt<curlopts::WriteStream>(&oss);
        request.perform();
        uint16_t ret = curlpp::infos::ResponseCode::get(request);
        if (ret == 200 || ret == 302 || ret == 307
            || ret == 301 || ret == 308)
        {
            answer = oss.str();
        }
        else
        {
            cerr << "HTTP error: " << std::to_string(ret) << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    return answer;
}

// Convert time_point to RFC 822 compliant time string.
const string strtime(const system_clock::time_point &timepoint)
{
    constexpr uint16_t bufsize = 1024;
    std::time_t time = system_clock::to_time_t(timepoint);
    std::tm *timeinfo;
    timeinfo = std::localtime(&time);
    char buffer[bufsize];
    std::strftime(buffer, bufsize, "%a, %d %b %Y %T %z", timeinfo);
    return static_cast<const string>(buffer);
}

// Convert ISO 8601 time string to RFC 822 time string.
const string strtime(const string &time)
{
    std::tm tm = {};
    std::stringstream ss(time);
    ss >> std::get_time(&tm, "%Y-%m-%dT%T%z");
    return strtime(std::chrono::system_clock::from_time_t(std::mktime(&tm)));
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "usage: " << argv[0] << " URL of Gitea project\n";
        return 1;
    }

    curlpp::initialize();

    string url = argv[1];
    size_t pos_repo = url.find('/', 8) + 1;
    const string baseurl = url.substr(0, pos_repo - 1);
    const string domain = baseurl.substr(baseurl.rfind('/') + 1);
    const string repo = url.substr(pos_repo);
    const string project = repo.substr(repo.find('/') + 1);
    const string now = strtime(system_clock::now());
    stringstream data(get_http(baseurl + "/api/v1/repos/"
                               + repo + "/releases"));
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
        cout <<
            "    <item>\n"
            "      <title>"
             << project << ": " << release["name"].asString() << "</title>\n"
            "      <link>" << baseurl << "/" << repo << "/releases</link>\n"
            "      <guid isPermaLink=\"false\">"
             << domain << " release " << release["id"].asString() << "</guid>\n"
            "      <pubDate>"
             << strtime(release["published_at"].asString()) << "</pubDate>\n"
            "      <description><![CDATA[<p><strong>" << type << "</strong></p>"
             << "<pre>\n" << release["body"].asString()
             << "\n</pre>]]></description>\n";

        // for (const Json::Value &file : release["assets"])
        // {
        //     cout << "      <enclosure "
        //          << "url=\"" << file["browser_download_url"].asString()
        //          << "\" length=\"" << file["size"].asString()
        //          << "\" type=\"application/octet-stream\"" << "/>\n";
        // }

        cout << "    </item>\n";
    }

    cout <<
        "  </channel>\n"
        "</rss>\n";

    return 0;
}
