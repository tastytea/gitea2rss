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
#include "version.hpp"
#include "gitea2rss.hpp"

using std::cout;
using std::cerr;
using std::endl;

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

