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


#ifndef _tuiserver_XMLParseErrorReporter_h_
#define _tuiserver_XMLParseErrorReporter_h_

#include <xercesc/sax/ErrorHandler.hpp>

namespace XERCES_CPP_NAMESPACE {
    class SAXParseException;
}

namespace tuiframework {

/**
 *  Provides an error handler reporter for XML parse errors.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class XMLParseErrorReporter : public XERCES_CPP_NAMESPACE::ErrorHandler {
public:
    XMLParseErrorReporter();
    virtual ~XMLParseErrorReporter();

        /// Reports a warning.
    virtual void warning(const XERCES_CPP_NAMESPACE::SAXParseException & saxParseException);
        /// Reports an error.
    virtual void error(const XERCES_CPP_NAMESPACE::SAXParseException & saxParseException);
        /// Reports a fatal error.
    virtual void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException & saxParseException);
        /// Does nothing.
    virtual void resetErrors();
};

}

#endif
