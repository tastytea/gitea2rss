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

namespace curlopts = curlpp::options;

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
    const string repo = url.substr(pos_repo);

    stringstream data(get_http(baseurl + "/api/v1/repos/"
                               + repo + "/releases"));
    Json::Value json;
    data >> json;
    for (const Json::Value &release : json)
    {
        cout << release["name"] << endl;
        // cout << release["body"] << endl;
    }

    return 0;
}
