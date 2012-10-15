/* DTrack2SDK: C++ header file, A.R.T. GmbH 3.5.07-22.5.08
 *
 * DTrack2: functions to receive and process DTrack UDP packets (ASCII protocol), as
 * well as to exchange DTrack2 TCP command strings.
 * Copyright (C) 2007-2008, Advanced Realtime Tracking GmbH
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * Version v0.1.3
 *
 * Purpose:
 *  - receives DTrack UDP packets (ASCII protocol) and converts them into easier to handle data
 *  - sends and receives DTrack2 commands (TCP)
 *  - DTrack2 network protocol due to: 'Technical Appendix DTrack v2.0'
 *  - for ARTtrack Controller versions v0.2 (and compatible versions)
 *  - tested under Linux (gcc) and MS Windows 2000/XP (MS Visual C++)
 *
 * Usage:
 *  - for Linux, Unix:
 *    - comment '#define OS_WIN', uncomment '#define OS_UNIX' in file 'DTrack2.cpp'
 *  - for MS Windows:
 *    - comment '#define OS_UNIX', uncomment '#define OS_WIN' in file 'DTrack2.cpp'
 *    - link with 'ws2_32.lib'
 *
 * $Id: DTrack2.hpp,v 1.4 2008/05/23 09:01:52 kurt Exp $
 */

#ifndef _ART_DTRACK2_H
#define _ART_DTRACK2_H

#include <vector>
#include <string>

// --------------------------------------------------------------------------
// Data types

// Standard body data (6DOF):
//  - currently not tracked bodies are getting a quality of -1

typedef struct{
	int id;               // id number (starting with 0)
	float quality;        // quality (0 <= qu <= 1, no tracking if -1)
	
	float loc[3];         // location (in mm)
	float rot[9];         // rotation matrix (column-wise)
} dtrack2_body_type;

// A.R.T. Flystick data (6DOF + buttons):
//  - currently not tracked bodies are getting a quality of -1
//  - note the maximum number of buttons and joystick values

#define DTRACK2_FLYSTICK_MAX_BUTTON    16  // maximum number of buttons
#define DTRACK2_FLYSTICK_MAX_JOYSTICK   8  // maximum number of joystick values

typedef struct{
	int id;               // id number (starting with 0)
	float quality;        // quality (0 <= qu <= 1, no tracking if -1)

	int num_button;       // number of buttons
	int button[DTRACK2_FLYSTICK_MAX_BUTTON];  // button state (1 pressed, 0 not pressed)
	                                          // (0 front, 1..n-1 right to left)
	int num_joystick;     // number of joystick values
	float joystick[DTRACK2_FLYSTICK_MAX_JOYSTICK];  // joystick value (-1 <= joystick <= 1) 
	                                                // (0 horizontal, 1 vertical)

	float loc[3];         // location (in mm)
	float rot[9];         // rotation matrix (column-wise)
} dtrack2_flystick_type;

// Measurement tool data (6DOF + buttons):
//  - currently not tracked bodies are getting a quality of -1
//  - note the maximum number of buttons

#define DTRACK2_MEATOOL_MAX_BUTTON    1  // maximum number of buttons

typedef struct{
	int id;               // id number (starting with 0)
	float quality;        // quality (0 <= qu <= 1, no tracking if -1)
	
	int num_button;       // number of buttons
	int button[DTRACK2_MEATOOL_MAX_BUTTON];  // button state (1 pressed, 0 not pressed)
	
	float loc[3];         // location (in mm)
	float rot[9];         // rotation matrix (column-wise)
} dtrack2_meatool_type;

// A.R.T. Fingertracking hand data (6DOF + fingers):
//  - currently not tracked bodies are getting a quality of -1

#define DTRACK2_HAND_MAX_FINGER    5  // maximum number of fingers

typedef struct{
	int id;               // id number (starting with 0)
	float quality;        // quality (0 <= qu <= 1, no tracking if -1)

	int lr;               // left (0) or right (1) hand
	int nfinger;          // number of fingers (maximum 5)

	float loc[3];         // back of the hand: location (in mm)
	float rot[9];         // back of the hand: rotation matrix (column-wise) 

	struct{
		float loc[3];            // finger: location (in mm)
		float rot[9];            // finger: rotation matrix (column-wise) 

		float radiustip;         // finger: radius of tip
		float lengthphalanx[3];  // finger: length of phalanxes; order: outermost, middle, innermost
		float anglephalanx[2];   // finger: angle between phalanxes
	} finger[DTRACK2_HAND_MAX_FINGER];  // order: thumb, index finger, middle finger, ...
} dtrack2_hand_type;

// Single marker data (3DOF):

typedef struct{
	int id;               // id number (starting with 1)
	float quality;        // quality (0 <= qu <= 1)
	
	float loc[3];         // location (in mm)
} dtrack2_marker_type;


// --------------------------------------------------------------------------
// Library class

class DTrack2
{
public:

// Constructor:
//
// server_host (i): TCP access to DTrack2 server: hostname or IP address of ARTtrack Controller (empty if not used)
// server_port (i): TCP access to DTrack2 server: port number of ARTtrack Controller (0 to use default port 50105)
//
// data_port (i): UDP port number to receive tracking data from ARTtrack Controller (0 if to be chosen)
//
// data_bufsize (i): size of buffer for UDP packets (in bytes)
// data_timeout_us (i): UDP timeout (receiving) in us (in micro second)
// server_timeout_us (i): TCP timeout for access to DTrack2 server (in micro second; receiving and sending)

	DTrack2(
		const std::string& server_host = "", unsigned short server_port = 0, unsigned short data_port = 0,
		int data_bufsize = 20000, int data_timeout_us = 1000000, int server_timeout_us = 10000000
	);

// Destructor:

	~DTrack2(void);

// Check if initialization was successfull:
//
// return value (o): boolean

	bool valid(void);

// Get used UDP port number:
//
// return value (o): port number

	unsigned short get_data_port(void);


// Check last data/server error:
//
// return value (o): boolean

	bool data_timeout(void);         // timeout while receiving tracking data
	bool data_neterror(void);        // network error while receiving tracking data
	bool data_parseerror(void);      // error while parsing tracking data

	bool server_noconnection(void);  // connection to DTrack2 server is completely lost
	bool server_timeout(void);       // server connection timed out
	bool server_neterror(void);      // server network error
	bool server_parseerror(void);    // error while parsing server answer


// Receive and process one DTrack2 data packet (UDP; ASCII protocol):
//
// return value (o): receiving was successfull

	bool receive(void);

// Get data of last received DTrack2 data packet:
//  - currently not tracked bodies are getting a quality of -1

	unsigned int get_framecounter(void);        // frame counter
	double get_timestamp(void);                 // timestamp (-1 if information not available)

	int get_num_body(void);                     // number of calibrated standard bodies (as far as known)
	dtrack2_body_type get_body(int id);         // standard body data (id (i): standard body id 0..max-1)

	int get_num_flystick(void);                 // number of calibrated Flysticks
	dtrack2_flystick_type get_flystick(int id); // Flystick data (id (i): Flystick id 0..max-1)

	int get_num_meatool(void);                  // number of calibrated measurement tools
	dtrack2_meatool_type get_meatool(int id);   // measurement tool data (id (i): tool id 0..max-1)

	int get_num_hand(void);                     // number of calibrated Fingertracking hands (as far as known)
	dtrack2_hand_type get_hand(int id);         // Fingertracking hand data (id (i): hand id 0..max-1)

	int get_num_marker(void);                   // number of tracked single markers
	dtrack2_marker_type get_marker(int index);  // single marker data (index (i): index 0..max-1)


// Set DTrack2 parameter:
//
// category (i): parameter category
// name (i): parameter name
// value (i): parameter value
// parameter (i): total parameter (category, name and value; without starting "dtrack2 set ")
//
// return value (o): setting was successfull (if not, a DTrack2 error message is available)

	bool set_parameter(const std::string& category, const std::string& name, const std::string& value);
	bool set_parameter(const std::string& parameter);

// Get DTrack2 parameter:
//
// category (i): parameter category
// name (i): parameter name
// parameter (i): total parameter (category and name; without starting "dtrack2 get ")
// value (o): parameter value
//
// return value (o): getting was successfull (if not, a DTrack2 error message is available)

	bool get_parameter(const std::string& category, const std::string& name, std::string& value);
	bool get_parameter(const std::string& parameter, std::string& value);

// Send DTrack2 command:
//
// command (i): command (without starting "dtrack2 ")
//
// return value (o): command was successfull (if not, a DTrack2 error message is available)

	bool send_command(const std::string& command);


// Get last DTrack2 error:
//
// errorcode (o): error as code number
// errorstring (o): error as string
//
// return value (o): error code was available, otherwise last command was successfull

	bool get_lasterror(int& errorcode);
	bool get_lasterror(std::string& errorstring);

// Get DTrack2 message:
//
// return value (o): message was available

	bool get_message(void);
	
// Get data of last DTrack2 message:

	std::string get_message_origin(void);    // origin of message
	std::string get_message_status(void);    // status of message
	unsigned int get_message_framenr(void);  // frame counter
	unsigned int get_message_errorid(void);  // error id
	std::string get_message_msg(void);       // message string


private:
	void* d_tcpsock;                // socket number for TCP
	int d_tcptimeout_us;            // timeout for receiving and sending TCP data

	void* d_udpsock;                // socket number for UDP
	unsigned short d_udpport;       // port number for UDP
	int d_udptimeout_us;            // timeout for receiving UDP data

	int d_udpbufsize;               // size of UDP buffer
	char* d_udpbuf;                 // UDP buffer

	unsigned int act_framecounter;                   // frame counter
	double act_timestamp;                            // timestamp (-1, if information not available)
	
	int act_num_body;                                // number of calibrated standard bodies (as far as known)
	std::vector<dtrack2_body_type> act_body;         // array containing standard body data

	int act_num_flystick;                            // number of calibrated Flysticks
	std::vector<dtrack2_flystick_type> act_flystick; // array containing Flystick data

	int act_num_meatool;                             // number of calibrated measurement tools
	std::vector<dtrack2_meatool_type> act_meatool;   // array containing measurement tool data

	int act_num_hand;                                // number of calibrated Fingertracking hands (as far as known)
	std::vector<dtrack2_hand_type> act_hand;         // array containing Fingertracking hands data

	int act_num_marker;                              // number of tracked single markers
	std::vector<dtrack2_marker_type> act_marker;     // array containing single marker data
	

	std::string d_message_origin;     // last DTrack2 message: origin of message
	std::string d_message_status;     // last DTrack2 message: status of message
	unsigned int d_message_framenr;   // last DTrack2 message: frame counter
	unsigned int d_message_errorid;   // last DTrack2 message: error id
	std::string d_message_msg;        // last DTrack2 message: message string

	enum{ err_none, err_timeout, err_net, err_parse } d_lastdataerror, d_lastservererror;  // last data/server error

	int d_lastdterrorcode;            // last DTrack2 error: as code
	std::string d_lastdterrorstring;  // last DTrack2 error: as string
	
	void set_data_noerror(void);       // set last data error to 'no error'
	void set_data_timeout(void);       // set last data error to 'timeout'
	void set_data_neterror(void);      // set last data error to 'network error'
	void set_data_parseerror(void);    // set last data error to 'parse error'

	void set_server_noerror(void);     // set last server error to 'no error'
	void set_server_timeout(void);     // set last server error to 'timeout'
	void set_server_neterror(void);    // set last server error to 'network error'
	void set_server_parseerror(void);  // set last server error to 'parse error'

	bool cmd_storelasterror(const char* res);         // store DTrack2 error return
	int cmd_sendreceive(const char* cmd, char* res);  // send command to DTrack2 and receive answer
};


// ---------------------------------------------------------------------------------------------------

#endif // _ART_DTRACK2_H


