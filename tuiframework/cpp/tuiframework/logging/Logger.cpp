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


#include "Logger.h"
#include <pthread.h>
#include <iostream>


using namespace std;

namespace tuiframework {

template<> Logger * LoggerSingleton::instance = 0;

Logger::Logger() {
}


Logger::~Logger() {
}


void Logger::registerThreadName(const std::string & name) {
    pthread_t tid = pthread_self();
#ifdef _WIN32
    long int key = reinterpret_cast<long int>(tid.p);
#else
    long int key = static_cast<long int>((long int) tid);
#endif
    string str = name;
    str.resize(8, ' ');
    this->threadNames[key] = str;
}



void Logger::Log(const std::string & msg, const std::string & file, int line) {

    pthread_t tid = pthread_self();
#ifdef _WIN32
    long int key = reinterpret_cast<long int>(tid.p);
#else
    long int key = static_cast<long int>((long int)tid);
#endif

    map<long int, string>::iterator iter = this->threadNames.find(key);
    string threadName;
    if (iter == this->threadNames.end()) {
        threadName = "";
        threadName.resize(8, ' ');
    } else {
        threadName = (*iter).second;
    }

	cout << "[" << threadName << "]" << " ";
    cout << msg;

    if (file.size() || line != -1) {
        cout << " [";
        if (file.size()) {
            cout << file;
            if (line != -1) {
                cout << ", " << line;
            }
        } else {
            cout << line;
        }

        cout << "]" << endl;
    }

}

std::string TF_SSCONVTOSTR(std::stringstream & ss) {
    std::string str = ss.str();
    ss.str("");
    return str;
}


}
