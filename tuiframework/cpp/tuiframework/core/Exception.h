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


#ifndef _tuiframework_Exception_h_
#define _tuiframework_Exception_h_

#include "ISerializable.h"

#include <iosfwd>
#include <string>

#include <list>

namespace tuiframework {

/**
 *  Represents an exception object used in the tuiframework environment.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class Exception : public ISerializable {
    /**
     * Stores the a message entry.
     * An Exception is able to store multiple entries.
     */
    struct Entry {
        Entry() {}
        Entry(const std::string & message,
                const std::string & file,
                int line);

            /// message text
        std::string message;
            /// the source file where the exception occurs
        std::string file;
            /// line number within the source file where the exception occurs
        int line;
    };

public:
        /// Constructor
    Exception();
        /// Construction with an error message.
    Exception(const std::string & errorMessage,
            const std::string & file = "",
            int line = -1);
        /// Destructor
    virtual ~Exception();

        /// Adds an error message
    void addErrorMessage(const std::string & errorMessage,
            const std::string & file = "",
            int line = -1);

    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

    std::string getFormattedString() const;

protected:
        /// stores the message entries
    std::list<Entry> entries;
};

}

#endif
