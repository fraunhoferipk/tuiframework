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


#ifndef _tuiserver_XMLNodeFilter_h_
#define _tuiserver_XMLNodeFilter_h_

#include <xercesc/dom/DOMNodeFilter.hpp>

namespace tuiframework {


/**
 *  Provides an XML node filtering.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class XMLNodeFilter : public XERCES_CPP_NAMESPACE::DOMNodeFilter {
public:
    XMLNodeFilter();
    virtual ~XMLNodeFilter();

        /// Returns a filter action as a function of the node type.
    virtual FilterAction acceptNode(const XERCES_CPP_NAMESPACE::DOMNode *) const;
};

}

#endif
