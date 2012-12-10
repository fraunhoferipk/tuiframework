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

#ifndef _tuiframework_Logger_h_
#define _tuiframework_Logger_h_

#include "../core/Singleton.h"
#include "SStreamSingleton.h"

#include <string>
#include <map>

#include <sstream>

namespace tuiframework {
 /**
 *  Logger
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class Logger {
public:
    Logger();
    virtual ~Logger();

    void Log(const std::string & msg, const std::string & file = "", int line = -1);
    void registerThreadName(const std::string & name);

protected:
    std::map<long int, std::string> threadNames;
};

typedef Singleton<Logger> LoggerSingleton;

std::string TF_SSCONVTOSTR(std::stringstream & ss);

#define TFSTR(a) TF_SSCONVTOSTR(static_cast<std::stringstream &>(SStreamSingleton::getSS() << a))
//#define TFSTR(a) TF_SSCONVTOSTR(static_cast<std::stringstream &>(SStreamSingleton::getSS() << a))

//#define LOG(level, a) { std::stringstream ss; ss << "[" << level << "] " << a << }

#define LOG(level, a) SStreamSingleton::getSS() << "[" << level << "] " << a; \
    LoggerSingleton::getInstance()->Log(SStreamSingleton::getSS().str(), __FILE__, __LINE__); \
    SStreamSingleton::getSS().str("");

#define TREG_THREADNAME(a) LoggerSingleton::getInstance()->registerThreadName(a);

#define LOG_IF(level, expression, a) if (expression) { LOG(level, a) }
#define LOG_IFELSE(level, expression, a, b) if (expression) { LOG(level, a) } else { LOG(level, b) }


/*
#define LOG_IF(level, expression, a) if (expression) { \
    LoggerSingleton::getInstance()->getIS() << "[" << level << "] " << a; \
    LoggerSingleton::getInstance()->Log(__FILE__, __LINE__); }

#define LOG_IFELSE(level, expression, a, b) if (expression) { \
    LoggerSingleton::getInstance()->getIS() << "[" << level << "] " << a; } else { \
    LoggerSingleton::getInstance()->getIS() << "[" << level << "] " << b; } \
    LoggerSingleton::getInstance()->Log(__FILE__, __LINE__);
    */

#define TFINFO(a) LOG("INFO   ", a)
#define TFINFO_IF(expression, a) LOG_IF("INFO   ", expression, a)
#define TFINFO_IFELSE(expression, a, b) LOG_IFELSE("INFO   ", expression, a, b)

#define TFWARNING(a) LOG("WARNING", a)
#define TFWARNING_IF(expression, a) LOG_IF("WARNING", expression, a)
#define TFWARNING_IFELSE(expression, a, b) LOG_IFELSE("WARNING", expression, a, b)

#define TFERROR(a) LOG("ERROR  ", a)
#define TFERROR_IF(expression, a) LOG_IF("ERROR  ", expression, a)
#define TFERROR_IFELSE(expression, a, b) LOG_IFELSE("ERROR  ", expression, a, b)

#define TFFATAL(a) LOG("FATAL  ", a)
#define TFFATAL_IF(expression, a) LOG_IF("FATAL  ", expression, a)
#define TFFATAL_IFELSE(expression, a, b) LOG_IFELSE("FATAL  ", expression, a, b)

#ifdef USE_TFDEBUG
    #define TFDEBUG(a) LOG("DEBUG  ", a)
    #define TFDEBUG_IF(expression, a) LOG_IF("DEBUG  ", expression, a)
    #define TFDEBUG_IFELSE(expression, a, b) LOG_IFELSE("DEBUG  ", expression, a, b)
#else
    #define TFDEBUG(a)
    #define TFDEBUG_IF(expression, a)
    #define TFDEBUG_IFELSE(expression, a, b)
#endif

/*
#define DEBUG(a) std::stringstream ss; ss << a; \
        LoggerSingleton::getInstance()->Log(ss.str(), __FILE__, __LINE__);
*/
// Condition

// __LINE__ __FILE__ __DATE__ __TIMESTAMP__
// __FUNC__
// __PRETTY_FUNCTION__ in Exceptions if available
// Stacktracer
// Thread ID

}

#endif

