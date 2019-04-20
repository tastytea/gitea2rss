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

#include <sstream>
#include <iostream>
#include <exception>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>
#include "version.hpp"
#include "gitea2rss.hpp"

namespace curlopts = curlpp::options;

using std::cout;
using std::cerr;
using std::endl;
using std::ostringstream;
using std::uint16_t;

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
            if (cgi)
            {
                cout << "Status: " << std::to_string(ret) << endl;
            }
            cerr << "HTTP Error: " << std::to_string(ret) << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    return answer;
}
