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


#include "ParameterGroup.h"

#include "../core/Exception.h"

#define USE_TFDEBUG

#include "../logging/Logger.h"

#include <stdlib.h>

using namespace std;

namespace tuiframework {


ParameterGroup::ParameterGroup(const std::string & name) :
    name(name) {
}


ParameterGroup::~ParameterGroup() {
    parameterGroupMap;
}


void ParameterGroup::setName(const std::string & name) {
    this->name = name;
}


const std::string & ParameterGroup::getName() const {
    return this->name;
}


void ParameterGroup::setParameterMap(const std::map<std::string, std::string> & parameterMap) {
    this->parameterMap = parameterMap;
}


const std::map<std::string, std::string> & ParameterGroup::getParameterMap() const {
    return this->parameterMap;
}


std::map<std::string, std::string> & ParameterGroup::getParameterMap() {
    return this->parameterMap;
}


void ParameterGroup::setParameterGroupMap(const std::map<std::string, ParameterGroup> & parameterGroupMap) {
    this->parameterGroupMap = parameterGroupMap;
}


const std::map<std::string, ParameterGroup> & ParameterGroup::getParameterGroupMap() const {
    return this->parameterGroupMap;
}


std::map<std::string, ParameterGroup> & ParameterGroup::getParameterGroupMap() {
    return this->parameterGroupMap;
}


int ParameterGroup::getInt(const std::string & path) const {
    return atoi(this->getString(path).c_str());
}


double ParameterGroup::getDouble(const std::string & path) const {
    return atof(this->getString(path).c_str());
}


const std::string & ParameterGroup::getString(const std::string & path) const {
    try {
        string str = path;
        size_t pos = str.find('|');
        if (pos == string::npos) {
            map<string, string>::const_iterator iter = this->parameterMap.find(str);
            if (iter == this->parameterMap.end()) {
                throw Exception(TFSTR("Parameter \"" << str << "\" not found."), __FILE__, __LINE__);
            }
            return (*iter).second;
        } else {
            string name = str.substr(0, pos);

            map<string, ParameterGroup>::const_iterator iter = this->parameterGroupMap.find(name);
            if (iter == this->parameterGroupMap.end()) {
                throw Exception(TFSTR("ParameterGroup \"" << name << "\" not found."), __FILE__, __LINE__);
            }

            str.erase(0, pos + 1);
            return (*iter).second.getString(str);
        }
    } catch (Exception & e) {
        e.addErrorMessage(TFSTR("in ParameterGroup\"" << this->name << "\""), __FILE__, __LINE__);
        throw e;
    }
}


const ParameterGroup & ParameterGroup::getParameterGroup(const std::string & path) const {
    try {
       string str = path;
       size_t pos = str.find('|');
       string name = str.substr(0, pos);
       map<string, ParameterGroup>::const_iterator iter = this->parameterGroupMap.find(name);
       if (iter == this->parameterGroupMap.end()) {
           throw Exception(TFSTR("ParameterGroup \"" << str << "\" not found."), __FILE__, __LINE__);
       }

       if (pos == string::npos) {
           return (*iter).second;
       } else {
           str.erase(0, pos + 1);
           if (str.size()) {
               return (*iter).second.getParameterGroup(str);
           } else {
               return (*iter).second;
           }
       }
   } catch (Exception & e) {
       e.addErrorMessage(TFSTR("in ParameterGroup\"" << this->name << "\""), __FILE__, __LINE__);
       throw e;
   }
}


}
