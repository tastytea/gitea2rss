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
#include <exception>
#include <memory>
#include <regex>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <Poco/Environment.h>
#include "version.hpp"
#include "gitea2rss.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::istream;
using std::unique_ptr;
using std::make_unique;
using std::regex;
using std::regex_search;
using std::smatch;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Environment;

void set_proxy()
{
    try
    {
        HTTPSClientSession::ProxyConfig proxyconfig;
        string env_proxy = Environment::get("http_proxy");
        regex re_proxy("^(?:https?://)?(?:([^:]+):?([^@]*)@)?" // user:password
                       "([^:]+):([[:digit:]]+/?)");            // host:port
        smatch match;

        if (regex_search(env_proxy, match, re_proxy))
        {
            string username, password;
            Poco::URI::decode(match[1].str(), username);
            Poco::URI::decode(match[2].str(), password);

            proxyconfig.host = match[3].str();
            proxyconfig.port = std::stoi(match[4].str());
            proxyconfig.username = username;
            proxyconfig.password = password;

            HTTPSClientSession::setGlobalProxyConfig(proxyconfig);
        }
    }
    catch (const std::exception &)
    {
        // No proxy found, no problem.
    }
}

const string get_http(const string &url)
{
    try
    {
        Poco::URI poco_uri(url);
        string path = poco_uri.getPathAndQuery();
        if (path.empty())
        {
            path = "/";
        }

        unique_ptr<HTTPClientSession> session;
        if (poco_uri.getScheme() == "https")
        {
            session = make_unique<HTTPSClientSession>(poco_uri.getHost(),
                                                      poco_uri.getPort());
        }
        else if (poco_uri.getScheme() == "http")
        {
            session = make_unique<HTTPClientSession>(poco_uri.getHost(),
                                                     poco_uri.getPort());
        }
        else
        {
            throw Poco::Exception("Protocol not supported.");
        }

        HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
        request.set("User-Agent", string("gitea2rss/") + global::version);

        HTTPResponse response;

        session->sendRequest(request);
        istream &rs = session->receiveResponse(response);

        // Not using the constants because some are too new for Debian stretch.
        switch (response.getStatus())
        {
        case 301:               // HTTPResponse::HTTP_MOVED_PERMANENTLY
        case 308:               // HTTPResponse::HTTP_PERMANENT_REDIRECT
        case 302:               // HTTPResponse::HTTP_FOUND
        case 303:               // HTTPResponse::HTTP_SEE_OTHER
        case 307:               // HTTPResponse::HTTP_TEMPORARY_REDIRECT
        {
            string location = response.get("Location");
            if (location.substr(0, 4) != "http")
            {
                location = poco_uri.getScheme() + "://" + poco_uri.getHost()
                    + location;
            }
            return get_http(location);
        }
        case HTTPResponse::HTTP_OK:
        {
            string answer;
            StreamCopier::copyToString(rs, answer);
            return answer;
        }
        default:
        {
            if (cgi)
            {
                cout << "Status: " << response.getStatus() << endl;
            }
            cerr << "HTTP Error: " << response.getStatus() << endl;

            return "";
        }
        }
    }
    catch (const Poco::Exception &e)
    {
        cerr << "Error: " << e.displayText() << endl;
    }

    return "";
}
