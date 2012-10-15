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


#include "XMLParseErrorReporter.h"

#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/util/XMLString.hpp>

#include <iostream>

using namespace XERCES_CPP_NAMESPACE;
using namespace std;

namespace tuiframework {

XMLParseErrorReporter::XMLParseErrorReporter() {
}


XMLParseErrorReporter::~XMLParseErrorReporter() {
}


void XMLParseErrorReporter::warning(const SAXParseException & spe) {
}


void XMLParseErrorReporter::error(const SAXParseException & spe) {
    cerr << "Error at file \"" << XMLString::transcode(spe.getSystemId())
         << "\", line " << spe.getLineNumber()
         << ", column " << spe.getColumnNumber()
         << "\n   Message: " << XMLString::transcode(spe.getMessage()) << endl;
}


void XMLParseErrorReporter::fatalError(const SAXParseException & spe) {
    cerr << "Fatal Error at file \"" << XMLString::transcode(spe.getSystemId())
         << "\", line " << spe.getLineNumber()
         << ", column " << spe.getColumnNumber()
         << "\n   Message: " << XMLString::transcode(spe.getMessage()) << endl;
}


void XMLParseErrorReporter::resetErrors() {
}

}

