/*
    Copyright (C) 2010, 2011, 2012 The Fraunhofer Institute for Production Systems and
    Design Technology IPK. All rights reserved.

    This file is part of the TUIFramework library.
    It includes a software framework which contains common code
    providing generic functionality for developing applications
    with a tangible user interface (TUI).
    
    The TUIFramework library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    The TUIFramework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with the TUIFramework.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "Exception.h"

#include <ostream>
#include <istream>
#include <sstream>

using namespace std;

namespace tuiframework {

Exception::Entry::Entry(
        const std::string & message,
        const std::string & file,
        int line) :
        message(message),
        file(file),
        line(line) {
}


Exception::Exception(
        const std::string & errorMessage,
        const std::string & file,
        int line) {
    this->addErrorMessage(errorMessage, file, line);
}


Exception::~Exception() {
}


void Exception::addErrorMessage(const std::string & errorMessage,
        const std::string & file,
        int line) {
    this->entries.push_back(Entry(errorMessage, file, line));
}


std::ostream & Exception::serialize(std::ostream & os) const {

    os << this->entries.size();

    list<Entry>::const_iterator i = this->entries.begin();
    list<Entry>::const_iterator e = this->entries.end();
    while (i != e) {
        //os << *i << endl;
        os << (*i).message.size() << " ";
        os.write((*i).message.c_str(), (*i).message.size());
        os << " " << (*i).file.size() << " ";
        os.write((*i).file.c_str(), (*i).file.size());
        os << " " << (*i).line;
        ++i;
    }

    return os;
}

static std::string readString(std::istream & is) {
    int size;
    is >> size;
    if (size > 0) {
        char * text = new char[size + 1];
        is.read(text, size);
        text[size] = 0;
        std::string str = text;
            // read the delimiter
        is.read(text, 1);
        delete text;
        return str;
    }
    return "";
}


std::istream & Exception::deSerialize(std::istream & is) {
    int count;
    is >> count;
    int i = 0;
    while (i < count) {
        Entry e;
        e.message = readString(is);
        e.file = readString(is);
        is >> e.line;
        ++i;
    }

    return is;
}


std::string Exception::getFormattedString() const {
    stringstream ss;
    ss << "TUIFramework Exception" << endl;
    list<Entry>::const_iterator i = this->entries.begin();
    list<Entry>::const_iterator e = this->entries.end();
    while (i != e) {
        ss << "  ";
        ss << (*i).message << endl;
        ss << "    " << "in file: " << (*i).file << ", line: " << (*i).line << endl;
        ++i;
    }
    return ss.str();
}

}

