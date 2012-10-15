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

#ifndef _tuiframework_ParameterGroup_h_
#define _tuiframework_ParameterGroup_h_

#include <map>
#include <string>

namespace tuiframework {

/**
 *  ParameterGroup
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class ParameterGroup {

public:
    ParameterGroup(const std::string & name = "");
    virtual ~ParameterGroup();

    void setName(const std::string & name);
    const std::string & getName() const;

    void setParameterMap(const std::map<std::string, std::string> & parameterMap);
    const std::map<std::string, std::string> & getParameterMap() const;
    std::map<std::string, std::string> & getParameterMap();

    void setParameterGroupMap(const std::map<std::string, ParameterGroup> & parameterGroupMap);
    const std::map<std::string, ParameterGroup> & getParameterGroupMap() const;
    std::map<std::string, ParameterGroup> & getParameterGroupMap();

    int getInt(const std::string & path) const;
    double getDouble(const std::string & path) const;
    const std::string & getString(const std::string & path) const;
    const ParameterGroup & getParameterGroup(const std::string & path) const;

protected:
    std::string name;
    std::map<std::string, std::string> parameterMap;
    std::map<std::string, ParameterGroup> parameterGroupMap;
};

}

#endif
