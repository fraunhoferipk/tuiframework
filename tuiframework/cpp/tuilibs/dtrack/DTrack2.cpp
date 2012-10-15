/* DTrack2SDK: C++ source file, A.R.T. GmbH 3.5.07-22.5.08
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
 * $Id: DTrack2.cpp,v 1.4 2008/05/23 09:06:53 kurt Exp $
 */

// usually the following should work; otherwise define OS_* manually:

#ifndef _WIN32
	#define OS_UNIX  // for Unix (Linux, Irix)
#else
	#define OS_WIN   // for MS Windows (2000, XP)
#endif

// --------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef OS_UNIX
	#include <unistd.h>
	#include <netdb.h>
	#include <sys/socket.h>
	#include <sys/time.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif
#ifdef OS_WIN
	#include <windows.h>
	#include <winsock.h>
#endif

#include "DTrack2.h"


// DTrack2 parameter:

#define DTRACK2_PROT_TCP_PORT  50105  // default port number
#define DTRACK2_PROT_MAXLEN      200  // max length of DTrack2 command string

// Local prototypes:

static char* string_nextline(char* str, char* start, int len);
static char* string_get_i(char* str, int* i);
static char* string_get_ui(char* str, unsigned int* ui);
static char* string_get_d(char* str, double* d);
static char* string_get_f(char* str, float* f);
static char* string_get_block(char* str, const char* fmt, int* idat, float* fdat);
static char* string_get_word(char* str, std::string& w);
static char* string_get_quoted_text(char* str, std::string& qt);
static char* string_cmp_parameter(char* str, const char* p);

static unsigned int ip_name2ip(const char* name);

static int udp_init(void** sock, unsigned short* port);
static int udp_exit(void* sock);
static int udp_receive(const void* sock, void *buffer, int maxlen, int tout_us);

static int tcp_client_init(void** sock, unsigned int ip, unsigned short port);
static int tcp_exit(void* sock);
static int tcp_receive(const void* sock, void *buffer, int maxlen, int tout_us);
static int tcp_send(const void* sock, const void* buffer, int len, int tout_us);


// --------------------------------------------------------------------------
// Library class

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

DTrack2::DTrack2(
	const std::string& server_host, unsigned short server_port, unsigned short data_port,
	int data_bufsize, int data_timeout_us, int server_timeout_us
)
{
	int err;
	unsigned int ip;

	d_udpsock = NULL;
	d_tcpsock = NULL;
	d_udpbuf = NULL;

	set_data_noerror();
	set_server_noerror();
	cmd_storelasterror(NULL);

	d_udptimeout_us = data_timeout_us;
	d_tcptimeout_us = server_timeout_us;

	// creat UDP socket:

	d_udpport = data_port;
	
	err = udp_init(&d_udpsock, &d_udpport);

	if(err){
		d_udpport = 0;
		return;
	}

	// creat UDP buffer:

	d_udpbufsize = data_bufsize;
	
	d_udpbuf = (char *)malloc(data_bufsize);
	
	if(!d_udpbuf){
		udp_exit(d_udpsock);
		d_udpsock = NULL;
		d_udpport = 0;
		return;
	}

	// access DTrack2 server:

	if(!server_host.empty()){
		ip = ip_name2ip(server_host.c_str());
		
		if(ip == 0){  // resolving of hostname was not possible
			free(d_udpbuf);
			d_udpbuf = NULL;
			udp_exit(d_udpsock);
			d_udpsock = NULL;
			d_udpport = 0;
			return;
		}
		
		err = tcp_client_init(&d_tcpsock, ip, (server_port == 0) ? DTRACK2_PROT_TCP_PORT : server_port);
		
		if(err){  // no connection to DTrack2 server
			free(d_udpbuf);
			d_udpbuf = NULL;
			udp_exit(d_udpsock);
			d_udpsock = NULL;
			d_udpport = 0;
			return;
		}
	}
	
	// reset actual DTrack data:

	act_framecounter = 0;
	act_timestamp = -1;

	act_num_body = act_num_flystick = act_num_meatool = act_num_hand = 0;
	act_num_marker = 0;

	d_message_origin = "";
	d_message_status = "";
	d_message_framenr = 0;
	d_message_errorid = 0;
	d_message_msg = "";
}


// Destructor:

DTrack2::~DTrack2(void)
{

	// release buffer:

	if(d_udpbuf){
		free(d_udpbuf);
	}

	// release sockets:

	if(d_udpsock){
		udp_exit(d_udpsock);
		d_udpsock = NULL;
	}

	if(d_tcpsock){
		tcp_exit(d_tcpsock);
		d_tcpsock = NULL;
	}
}


// Check if initialization was successfull:
//
// return value (o): boolean

bool DTrack2::valid(void)
{
	return (d_udpsock != NULL);
}


// Get used UDP port number:
//
// return value (o): port number

unsigned short DTrack2::get_data_port(void)
{
	return d_udpport;
}


// Check last data error:
//
// return value (o): boolean

bool DTrack2::data_timeout(void)         // timeout while receiving tracking data
{
	return (d_lastdataerror == err_timeout);
}

bool DTrack2::data_neterror(void)        // network error while receiving tracking data
{
	return (d_lastdataerror == err_net);
}

bool DTrack2::data_parseerror(void)      // error while parsing tracking data
{
	return (d_lastdataerror == err_parse);
}


// Check last server error:
//
// return value (o): boolean

bool DTrack2::server_noconnection(void)  // connection to DTrack2 server is completely lost
{
	return (d_tcpsock == NULL);
}

bool DTrack2::server_timeout(void)       // server connection timed out
{
	return (d_lastservererror == err_timeout);
}

bool DTrack2::server_neterror(void)      // server network error
{
	return (d_lastservererror == err_net);
}

bool DTrack2::server_parseerror(void)    // error while parsing server answer
{
	return (d_lastservererror == err_parse);
}


// Set last data error:

void DTrack2::set_data_noerror(void)       // 'no error'
{
	d_lastdataerror = err_none;
}

void DTrack2::set_data_timeout(void)       // 'timeout'
{
	d_lastdataerror = err_timeout;
}

void DTrack2::set_data_neterror(void)      // 'network error'
{
	d_lastdataerror = err_net;
}

void DTrack2::set_data_parseerror(void)    // 'parse error'
{
	d_lastdataerror = err_parse;
}


// Set last server error:

void DTrack2::set_server_noerror(void)     // 'no error'
{
	d_lastservererror = err_none;
}

void DTrack2::set_server_timeout(void)     // 'timeout'
{
	d_lastservererror = err_timeout;
}

void DTrack2::set_server_neterror(void)    // 'network error'
{
	d_lastservererror = err_net;
}

void DTrack2::set_server_parseerror(void)  // 'parse error'
{
	d_lastservererror = err_parse;
}


// --------------------------------------------------------------------------
// Receive and process one DTrack2 data packet (UDP; ASCII protocol):
//
// return value (o): receiving was successfull

bool DTrack2::receive(void)
{
	char* s;
	int i, j, k, l, n, len, id;
	char sfmt[20];
	int iarr[3];
	float f, farr[6];
	int loc_num_bodycal, loc_num_handcal, loc_num_flystick1, loc_num_meatool;

	if(!valid()){
		set_data_neterror();
		return false;
	}

	// defaults:
	
	act_framecounter = 0;
	act_timestamp = -1;   // i.e. not available
	
	loc_num_bodycal = loc_num_handcal = -1;  // i.e. not available
	loc_num_flystick1 = loc_num_meatool = 0;
	
	// receive UDP packet:

	len = udp_receive(d_udpsock, d_udpbuf, d_udpbufsize-1, d_udptimeout_us);

	if(len == -1){
		set_data_timeout();
		return false;
	}
	if(len <= 0){
		set_data_neterror();
		return false;
	}

	s = d_udpbuf;
	s[len] = '\0';

	// process lines:

	set_data_parseerror();

	do{
		// line for frame counter:

		if(!strncmp(s, "fr ", 3)){
			s += 3;
			
			if(!(s = string_get_ui(s, &act_framecounter))){  // get frame counter
				act_framecounter = 0;
				return false;
			}

			continue;
		}

		// line for timestamp:

		if(!strncmp(s, "ts ", 3)){
			s += 3;
			
			if(!(s = string_get_d(s, &act_timestamp))){   // get timestamp
				act_timestamp = -1;
				return false;
			}

			continue;
		}
		
		// line for additional information about number of calibrated bodies:

		if(!strncmp(s, "6dcal ", 6)){
			s += 6;
			
			if(!(s = string_get_i(s, &loc_num_bodycal))){  // get number of calibrated bodies
				return false;
			}

			continue;
		}

		// line for standard body data:

		if(!strncmp(s, "6d ", 3)){
			s += 3;
			
			for(i=0; i<act_num_body; i++){  // disable all existing data
				memset(&act_body[i], 0, sizeof(dtrack2_body_type));
				act_body[i].id = i;
				act_body[i].quality = -1;
			}

			if(!(s = string_get_i(s, &n))){               // get number of standard bodies (in line)
				return false;
			}

			for(i=0; i<n; i++){                           // get data of standard bodies
				if(!(s = string_get_block(s, "if", &id, &f))){
					return false;
				}

				if(id >= act_num_body){  // adjust length of vector
					act_body.resize(id + 1);

					for(j=act_num_body; j<=id; j++){
						memset(&act_body[j], 0, sizeof(dtrack2_body_type));
						act_body[j].id = j;
						act_body[j].quality = -1;
					}

					act_num_body = id + 1;
				}
				
				act_body[id].id = id;
				act_body[id].quality = f;
				
				if(!(s = string_get_block(s, "fff", NULL, act_body[id].loc))){
					return false;
				}
			
				if(!(s = string_get_block(s, "fffffffff", NULL, act_body[id].rot))){
					return false;
				}
			}
			
			continue;
		}
		
		// line for Flystick data (older format):

		if(!strncmp(s, "6df ", 4)){
			s += 4;
			
			if(!(s = string_get_i(s, &n))){               // get number of calibrated Flysticks
				return false;
			}

			loc_num_flystick1 = n;
			
			if(n != act_num_flystick){  // adjust length of vector
				act_flystick.resize(n);
				
				act_num_flystick = n;
			}
			
			for(i=0; i<n; i++){                           // get data of Flysticks
				if(!(s = string_get_block(s, "ifi", iarr, &f))){
					return false;
				}
					
				if(iarr[0] != i){  // not expected
					return false;
				}

				act_flystick[i].id = iarr[0];
				act_flystick[i].quality = f;

				act_flystick[i].num_button = 8;
				k = iarr[1];
				for(j=0; j<8; j++){
					act_flystick[i].button[j] = k & 0x01;
					k >>= 1;
				}
				
				act_flystick[i].num_joystick = 2;  // additionally to buttons 5-8
				if(iarr[1] & 0x20){
					act_flystick[i].joystick[0] = -1;
				}else if(iarr[1] & 0x80){
					act_flystick[i].joystick[0] = 1;
				}else{
					act_flystick[i].joystick[0] = 0;
				}
				if(iarr[1] & 0x10){
					act_flystick[i].joystick[1] = -1;
				}else if(iarr[1] & 0x40){
					act_flystick[i].joystick[1] = 1;
				}else{
					act_flystick[i].joystick[1] = 0;
				}
				
				if(!(s = string_get_block(s, "fff", NULL, act_flystick[i].loc))){
					return false;
				}
				
				if(!(s = string_get_block(s, "fffffffff", NULL, act_flystick[i].rot))){
					return false;
				}
			}
			
			continue;
		}
		
		// line for Flystick data (newer format):

		if(!strncmp(s, "6df2 ", 5)){
			s += 5;
			
			if(!(s = string_get_i(s, &n))){               // get number of calibrated Flysticks
				return false;
			}

			if(n != act_num_flystick){  // adjust length of vector
				act_flystick.resize(n);
				
				act_num_flystick = n;
			}
			
			if(!(s = string_get_i(s, &n))){               // get number of Flysticks
				return false;
			}

			for(i=0; i<n; i++){                           // get data of Flysticks
				if(!(s = string_get_block(s, "ifii", iarr, &f))){
					return false;
				}
					
				if(iarr[0] != i){  // not expected
					return false;
				}

				act_flystick[i].id = iarr[0];
				act_flystick[i].quality = f;

				if(iarr[1] > DTRACK2_FLYSTICK_MAX_BUTTON){
					return false;
				}
				if(iarr[1] > DTRACK2_FLYSTICK_MAX_JOYSTICK){
					return false;
				}
				act_flystick[i].num_button = iarr[1];
				act_flystick[i].num_joystick = iarr[2];
				
				if(!(s = string_get_block(s, "fff", NULL, act_flystick[i].loc))){
					return false;
				}
				
				if(!(s = string_get_block(s, "fffffffff", NULL, act_flystick[i].rot))){
					return false;
				}

				strcpy(sfmt, "");
				j = 0;
				while(j < act_flystick[i].num_button){
					strcat(sfmt, "i");
					j += 32;
				}
				j = 0;
				while(j < act_flystick[i].num_joystick){
					strcat(sfmt, "f");
					j++;
				}
				
				if(!(s = string_get_block(s, sfmt, iarr, act_flystick[i].joystick))){
					return false;
				}

				k = l = 0;
				for(j=0; j<act_flystick[i].num_button; j++){
					act_flystick[i].button[j] = iarr[k] & 0x01;
					iarr[k] >>= 1;
					
					l++;
					if(l == 32){
						k++;
						l = 0;
					}
				}
			}
			
			continue;
		}

		// line for measurement tool data:

		if(!strncmp(s, "6dmt ", 5)){
			s += 5;
			
			if(!(s = string_get_i(s, &n))){               // get number of calibrated measurement tools
				return false;
			}

			loc_num_meatool = n;

			if(n != act_num_meatool){  // adjust length of vector
				act_meatool.resize(n);
				
				act_num_meatool = n;
			}
			
			for(i=0; i<n; i++){                           // get data of measurement tools
				if(!(s = string_get_block(s, "ifi", iarr, &f))){
					return false;
				}
				
				if(iarr[0] != i){  // not expected
					return false;
				}

				act_meatool[i].id = iarr[0];
				act_meatool[i].quality = f;

				act_meatool[i].num_button = 1;
				act_meatool[i].button[0] = iarr[1] & 0x01;
				
				if(!(s = string_get_block(s, "fff", NULL, act_meatool[i].loc))){
					return false;
				}
				
				if(!(s = string_get_block(s, "fffffffff", NULL, act_meatool[i].rot))){
					return false;
				}
			}
			
			continue;
		}
		
		// line for additional information about number of calibrated Fingertracking hands:

		if(!strncmp(s, "glcal ", 6)){
			s += 6;
			
			if(!(s = string_get_i(s, &loc_num_handcal))){  // get number of calibrated hands
				return false;
			}
			
			continue;
		}

		// line for A.R.T. Fingertracking hand data:

		if(!strncmp(s, "gl ", 3)){
			s += 3;
			
			for(i=0; i<act_num_hand; i++){  // disable all existing data
				memset(&act_hand[i], 0, sizeof(dtrack2_hand_type));
				act_hand[i].id = i;
				act_hand[i].quality = -1;
			}

			if(!(s = string_get_i(s, &n))){               // get number of hands (in line)
				return false;
			}

			for(i=0; i<n; i++){                           // get data of hands
				if(!(s = string_get_block(s, "ifii", iarr, &f))){
					return false;
				}
				
				id = iarr[0];

				if(id >= act_num_hand){  // adjust length of vector
					act_hand.resize(id + 1);

					for(j=act_num_hand; j<=id; j++){
						memset(&act_hand[j], 0, sizeof(dtrack2_hand_type));
						act_hand[j].id = j;
						act_hand[j].quality = -1;
					}
					
					act_num_hand = id + 1;
				}
				
				act_hand[id].id = iarr[0];
				act_hand[id].lr = iarr[1];
				act_hand[id].quality = f;
				
				if(iarr[2] > DTRACK2_HAND_MAX_FINGER){
					return false;
				}
				act_hand[id].nfinger = iarr[2];

				if(!(s = string_get_block(s, "fff", NULL, act_hand[id].loc))){
					return false;
				}
				
				if(!(s = string_get_block(s, "fffffffff", NULL, act_hand[id].rot))){
					return false;
				}

				for(j=0; j<act_hand[id].nfinger; j++){     // get data of fingers
					if(!(s = string_get_block(s, "fff", NULL, act_hand[id].finger[j].loc))){
						return false;
					}
			
					if(!(s = string_get_block(s, "fffffffff", NULL, act_hand[id].finger[j].rot))){
						return false;
					}

					if(!(s = string_get_block(s, "ffffff", NULL, farr))){
						return false;
					}

					act_hand[id].finger[j].radiustip = farr[0];
					act_hand[id].finger[j].lengthphalanx[0] = farr[1];
					act_hand[id].finger[j].anglephalanx[0] = farr[2];
					act_hand[id].finger[j].lengthphalanx[1] = farr[3];
					act_hand[id].finger[j].anglephalanx[1] = farr[4];
					act_hand[id].finger[j].lengthphalanx[2] = farr[5];
				}
			}
			
			continue;
		}
		
		// line for single marker data:

		if(!strncmp(s, "3d ", 3)){
			s += 3;
			
			if(!(s = string_get_i(s, &act_num_marker))){  // get number of markers
				act_num_marker = 0;
				return false;
			}

			if(act_num_marker > (int )act_marker.size()){
				act_marker.resize(act_num_marker);
			}

			for(i=0; i<act_num_marker; i++){              // get data of single markers
				if(!(s = string_get_block(s, "if", &act_marker[i].id, &act_marker[i].quality))){
					return false;
				}

				if(!(s = string_get_block(s, "fff", NULL, act_marker[i].loc))){
					return false;
				}
			}
			
			continue;
		}

		// ignore unknown line identifiers (could be valid in future DTracks)
		
	}while((s = string_nextline(d_udpbuf, s, d_udpbufsize)));

	// set number of calibrated standard bodies, if necessary:

	if(loc_num_bodycal >= 0){  // '6dcal' information was available
		n = loc_num_bodycal - loc_num_flystick1 - loc_num_meatool;
		
		if(n > act_num_body){  // adjust length of vector
			act_body.resize(n);

			for(j=act_num_body; j<n; j++){
				memset(&act_body[j], 0, sizeof(dtrack2_body_type));
				act_body[j].id = j;
				act_body[j].quality = -1;
			}
		}

		act_num_body = n;
	}

	// set number of calibrated Fingertracking hands, if necessary:

	if(loc_num_handcal >= 0){  // 'glcal' information was available
		if(loc_num_handcal > act_num_hand){  // adjust length of vector
			act_hand.resize(loc_num_handcal);

			for(j=act_num_hand; j<loc_num_handcal; j++){
				memset(&act_hand[j], 0, sizeof(dtrack2_hand_type));
				act_hand[j].id = j;
				act_hand[j].quality = -1;
			}
		}

		act_num_hand = loc_num_handcal;
	}
	
	set_data_noerror();
	return true;
}


// Get data of last received DTrack2 data packet:
//  - currently not tracked bodies are getting a quality of -1

unsigned int DTrack2::get_framecounter(void)  // frame counter
{
	return act_framecounter;
}

double DTrack2::get_timestamp(void)          // timestamp (-1 if information not available)
{
	return act_timestamp;
}


int DTrack2::get_num_body(void)              // number of calibrated standard bodies (as far as known)
{
	return act_num_body;  // best information known
}

dtrack2_body_type DTrack2::get_body(int id)  // standard body data (id (i): standard body id 0..max-1)
{

	if(id < act_num_body){
		return act_body[id];
	}
	
	dtrack2_body_type dummy;
	
	memset(&dummy, 0, sizeof(dtrack2_body_type));
	dummy.id = id;
	dummy.quality = -1;
	
	return dummy;
}


int DTrack2::get_num_flystick(void)          // number of calibrated Flysticks
{
	return act_num_flystick;
}

dtrack2_flystick_type DTrack2::get_flystick(int id) // Flystick data (id (i): Flystick id 0..max-1)
{

	if(id < act_num_flystick){
		return act_flystick[id];
	}
	
	dtrack2_flystick_type dummy;
	
	memset(&dummy, 0, sizeof(dtrack2_flystick_type));
	dummy.id = id;
	dummy.quality = -1;

	return dummy;
}


int DTrack2::get_num_meatool(void)           // number of calibrated measurement tools
{
	return act_num_meatool;
}

dtrack2_meatool_type DTrack2::get_meatool(int id) // measurement tool data (id (i): tool id 0..max-1)
{

	if(id < act_num_meatool){
		return act_meatool[id];
	}
	
	dtrack2_meatool_type dummy;
	
	memset(&dummy, 0, sizeof(dtrack2_meatool_type));
	dummy.id = id;
	dummy.quality = -1;

	return dummy;
}


int DTrack2::get_num_hand(void)              // number of calibrated Fingertracking hands (as far as known)
{
	return act_num_hand;  // best information known
}

dtrack2_hand_type DTrack2::get_hand(int id)  // Fingertracking hand data (id (i): hand id 0..max-1)
{

	if(id < act_num_hand){
		return act_hand[id];
	}
	
	dtrack2_hand_type dummy;
	
	memset(&dummy, 0, sizeof(dtrack2_hand_type));
	dummy.id = id;
	dummy.quality = -1;
	
	return dummy;
}


int DTrack2::get_num_marker(void)            // number of tracked single markers
{
	return act_num_marker;
}

dtrack2_marker_type DTrack2::get_marker(int index) // single marker data (index (i): index 0..max-1)
{

	if(index >= 0 && index < act_num_marker){
		return act_marker[index];
	}
	
	dtrack2_marker_type dummy;
	
	memset(&dummy, 0, sizeof(dtrack2_marker_type));
	dummy.id = 0;
	dummy.quality = -1;
	
	return dummy;
}


// --------------------------------------------------------------------------
// Access to DTrack2 server:

// Set DTrack2 parameter:
//
// category (i): parameter category
// name (i): parameter name
// value (i): parameter value
//
// return value (o): setting was successfull (if not, a DTrack2 error message is available)

bool DTrack2::set_parameter(const std::string& category, const std::string& name, const std::string& value)
{
	std::string cmd;

	cmd = "set " + category;
	cmd += " " + name;
	cmd += " " + value;

	return send_command(cmd);
}


// Set DTrack2 parameter:
//
// parameter (i): total parameter (category, name and value; without starting "dtrack2 set ")
//
// return value (o): setting was successfull (if not, a DTrack2 error message is available)

bool DTrack2::set_parameter(const std::string& parameter)
{

	return send_command("set " + parameter);
}


// Get DTrack2 parameter:
//
// category (i): parameter category
// name (i): parameter name
// value (o): parameter value
//
// return value (o): getting was successfull (if not, a DTrack2 error message is available)

bool DTrack2::get_parameter(const std::string& category, const std::string& name, std::string& value)
{
	std::string cmd;

	cmd = category;
	cmd += " " + name;

	return get_parameter(cmd, value);
}


// Get DTrack2 parameter:
//
// parameter (i): total parameter (category and name; without starting "dtrack2 get ")
// value (o): parameter value
//
// return value (o): getting was successfull (if not, a DTrack2 error message is available)

bool DTrack2::get_parameter(const std::string& parameter, std::string& value)
{
	std::string cmd;
	char res[DTRACK2_PROT_MAXLEN];
	char* s;
	int n;

	cmd = "dtrack2 get " + parameter;
	
	value = "";
	cmd_storelasterror(NULL);

	if(cmd_sendreceive(cmd.c_str(), res)){
		return false;
	}

	n = (int )strlen("dtrack2 set ");
	
	if(!strncmp(res, "dtrack2 set ", n)){
		s = res + n;

		if(!(s = string_cmp_parameter(s, parameter.c_str()))){  // check 'parameter'
			set_server_parseerror();
			return false;
		}

		value = s;
		return true;
	}

	return cmd_storelasterror(res);  // store error message
}


// Send DTrack2 command:
//
// command (i): command (without starting "dtrack2 ")
//
// return value (o): command was successfull (if not, a DTrack2 error message is available)

bool DTrack2::send_command(const std::string& command)
{
	std::string cmd;
	char res[DTRACK2_PROT_MAXLEN+1];

	cmd = "dtrack2 " + command;

	cmd_storelasterror(NULL);

	if(cmd_sendreceive(cmd.c_str(), res)){
		return false;
	}

	return cmd_storelasterror(res);  // store error message
}


// Get last DTrack2 error:
//
// errorcode (o): error as code number
// errorstring (o): error as string
//
// return value (o): error code was available, otherwise last command was successfull

bool DTrack2::get_lasterror(int& errorcode)
{

	if(!d_lastdterrorcode){  // no error available
		return false;
	}

	errorcode = d_lastdterrorcode;
	return true;
}

bool DTrack2::get_lasterror(std::string& errorstring)
{

	if(!d_lastdterrorcode){  // no error available
		return false;
	}

	errorstring = d_lastdterrorstring;
	return true;
}


// Store DTrack2 error return:
// res (i): answer string (NULL to reset to 'no error')
// return value (o): error return shows successfull command

bool DTrack2::cmd_storelasterror(const char* res)
{
	int i, n;
	const char* s;

	if(!res){
		d_lastdterrorcode = 0;
		d_lastdterrorstring = "no error";
		return true;
	}

	if(!strcmp(res, "dtrack2 ok")){  // no error
		d_lastdterrorcode = 0;
		d_lastdterrorstring = "no error";
		return true;
	}

	n = (int )strlen("dtrack2 err ");
	
	if(!strncmp(res, "dtrack2 err ", n)){
		s = res + n;

		if(!(s = string_get_i((char *)s, &i))){  // get 'error code'
			d_lastdterrorcode = -1111;
			d_lastdterrorstring = "internal SDK error";

			set_server_parseerror();
			return false;
		}
		
		d_lastdterrorcode = i;

		if(!(s = string_get_quoted_text((char *)s, d_lastdterrorstring))){  // get 'error string'
			d_lastdterrorcode = -1111;
			d_lastdterrorstring = "internal SDK error";

			set_server_parseerror();
			return false;
		}

		return false;
	}

	return false;
}


// Get DTrack2 message:
//
// return value (o): message was available

bool DTrack2::get_message(void)
{
	int n;
	char res[DTRACK2_PROT_MAXLEN];
	const char* s;
	unsigned int ui;

	cmd_storelasterror(NULL);

	if(cmd_sendreceive("dtrack2 getmsg", res)){
		return false;
	}

	if(!strcmp(res, "dtrack2 ok")){
		cmd_storelasterror(res);
		return false;  // no message available
	}

	n = (int )strlen("dtrack2 msg ");
	
	if(!strncmp(res, "dtrack2 msg ", n)){  // message
		s = res + n;

		if(!(s = string_get_word((char *)s, d_message_origin))){  // get 'origin'
			return false;
		}

		if(!(s = string_get_word((char *)s, d_message_status))){  // get 'status'
			return false;
		}

		if(!(s = string_get_ui((char *)s, &ui))){  // get 'frame counter'
			return false;
		}

		d_message_framenr = ui;

		if(!(s = string_get_ui((char *)s, &ui))){  // get 'error id'
			return false;
		}

		d_message_errorid = ui;

		if(!(s = string_get_quoted_text((char *)s, d_message_msg))){  // get 'message'
			return false;
		}
		
		cmd_storelasterror(NULL);
		return true;
	}
	
	cmd_storelasterror(res);  // store error message
	return false;
}


// Get data of last DTrack2 message:

std::string DTrack2::get_message_origin(void)    // origin of message
{
	return d_message_origin;
}

std::string DTrack2::get_message_status(void)    // status of message
{
	return d_message_status;
}

unsigned int DTrack2::get_message_framenr(void)  // frame counter
{
	return d_message_framenr;
}

unsigned int DTrack2::get_message_errorid(void)  // error id
{
	return d_message_errorid;
}

std::string DTrack2::get_message_msg(void)       // message string
{
	return d_message_msg;
}


// Send command to DTrack2 and receive answer:
// cmd (i): command string
// res (o): answer string
// return value (o): 0 exchange was ok, <0 if error occured

int DTrack2::cmd_sendreceive(const char* cmd, char* res)
{
	int len;

	len = (int )strlen(cmd);
	if(len > DTRACK2_PROT_MAXLEN){
		set_server_neterror();
		return -1;
	}

	if(!d_tcpsock){
		set_server_neterror();
		return -11;
	}

	// send TCP command string:

	if((len = tcp_send(d_tcpsock, cmd, len+1, d_tcptimeout_us))){
		set_server_neterror();  // network error
		return -11;
	}

	// receive TCP response string:

	if((len = tcp_receive(d_tcpsock, res, DTRACK2_PROT_MAXLEN, d_tcptimeout_us)) < 0){
		if(len == -1){  // timeout
			set_server_timeout();
		} else if(len == -9){  // broken connection
			tcp_exit(d_tcpsock);
			d_tcpsock = NULL;
		}else{  // network error
			set_server_neterror();
		}

		strcpy(res, "");
		return len;
	}
	
	res[len] = '\0';  // for safety

	set_server_noerror();
	return 0;
}


// ---------------------------------------------------------------------------------------------------
// Parsing DTrack data:

// Search next line in buffer:
// str (i): buffer (total)
// start (i): start position within buffer
// len (i): buffer length in bytes
// return (i): begin of line, NULL if no new line in buffer

static char* string_nextline(char* str, char* start, int len)
{
	char* s = start;
	char* se = str + len;
	int crlffound = 0;

	while(s < se){
		if(*s == '\r' || *s == '\n'){  // crlf
			crlffound = 1;
		}else{
			if(crlffound){              // begin of new line found
				return (*s) ? s : NULL;  // first character is '\0': end of buffer
			}
		}

		s++;
	}

	return NULL;                      // no new line found in buffer
}


// Read next 'int' value from string:
// str (i): string
// i (o): read value
// return value (o): pointer behind read value in str; NULL in case of error

static char* string_get_i(char* str, int* i)
{
	char* s;
	
	*i = (int )strtol(str, &s, 0);
	return (s == str) ? NULL : s;
}


// Read next 'unsigned int' value from string:
// str (i): string
// ui (o): read value
// return value (o): pointer behind read value in str; NULL in case of error

static char* string_get_ui(char* str, unsigned int* ui)
{
	char* s;
	
	*ui = (unsigned int )strtoul(str, &s, 0);
	return (s == str) ? NULL : s;
}


// Read next 'double' value from string:
// str (i): string
// d (o): read value
// return value (o): pointer behind read value in str; NULL in case of error

static char* string_get_d(char* str, double* d)
{
	char* s;
	
	*d = strtod(str, &s);
	return (s == str) ? NULL : s;
}


// Read next 'float' value from string:
// str (i): string
// f (o): read value
// return value (o): pointer behind read value in str; NULL in case of error

static char* string_get_f(char* str, float* f)
{
	char* s;
	
	*f = (float )strtod(str, &s);   // strtof() only available in GNU-C
	return (s == str) ? NULL : s;
}


// Process next block '[...]' in string:
// str (i): string
// fmt (i): format string ('i' for 'int', 'f' for 'float')
// idat (o): array for 'int' values (long enough due to fmt)
// fdat (o): array for 'float' values (long enough due to fmt)
// return value (o): pointer behind read value in str; NULL in case of error

static char* string_get_block(char* str, const char* fmt, int* idat, float* fdat)
{
	char* strend;
	int index_i, index_f;

	if((str = strchr(str, '[')) == NULL){       // search begin of block
		return NULL;
	}
	if((strend = strchr(str, ']')) == NULL){    // search end of block
		return NULL;
	}
	
	str++;                               // remove delimiters
	*strend = '\0';

	index_i = index_f = 0;

	while(*fmt){
		switch(*fmt++){
			case 'i':
				if((str = string_get_i(str, &idat[index_i++])) == NULL){
					*strend = ']';
					return NULL;
				}
				break;
				
			case 'f':
				if((str = string_get_f(str, &fdat[index_f++])) == NULL){
					*strend = ']';
					return NULL;
				}
				break;
				
			default:    // unknown format character
				*strend = ']';
				return NULL;
		}
	}

	// ignore additional data inside the block
	
	*strend = ']';
	return strend + 1;
}


// Read next 'word' value from string:
// str (i): string
// w (o): read value
// return value (o): pointer behind read value in str; NULL in case of error

static char* string_get_word(char* str, std::string& w)
{
	char* strend;

	while(*str == ' '){  // search begin of 'word'
		str++;
	}

	if(!(strend = strchr(str, ' '))){  // search end of 'word'
		w.assign(str);

		strend = str;
		while(*strend != '\0'){  // search end of 'word'
			strend++;
		}

		return (strend == str) ? NULL : strend;
	}
		
	w.assign(str, (int )(strend - str));

	return strend;
}


// Read next 'quoted text' value from string:
// str (i): string
// qt (o): read value (without quotes)
// return value (o): pointer behind read value in str; NULL in case of error

static char* string_get_quoted_text(char* str, std::string& qt)
{
	char* strend;

	if(!(str = strchr(str, '\"'))){  // search begin of 'quoted text'
		return NULL;
	}
	str++;
	
	if(!(strend = strchr(str, '\"'))){  // search end of 'quoted text'
		return NULL;
	}

	qt.assign(str, (int )(strend - str));
	
	return strend + 1;
}


// Compare strings regarding DTrack2 parameter rules:
// str (i): string
// p (i): parameter string
// return value (o): pointer behind parameter in str; NULL in case of error

static char* string_cmp_parameter(char* str, const char* p)
{
	bool lastwasdigit = false;

	while(*p){
		if(!lastwasdigit){  // skip leading zeros
			while(*p == '0'){
				p++;
			}
			
			while(*str == '0'){
				str++;
			}
		}
		
		if(*str != *p){  // compare next character
			return NULL;
		}

		if(*p == ' '){  // skip white space
			do{
				p++;
			}while(*p == ' ');
			
			do{
				str++;
			}while(*str == ' ');

			lastwasdigit = false;
			continue;
		}

		lastwasdigit = (*p >= '0' && *p <= '9');

		str++;
		p++;
	}

	do{  // skip white space
		str++;
	}while(*str == ' ');
	
	return str;
}


// ---------------------------------------------------------------------------------------------------
// General IP routines:

struct _ip_socket_struct{  // internal socket type
#ifdef OS_UNIX
	int ossock;     // Unix socket
#endif
#ifdef OS_WIN
	SOCKET ossock;  // Windows Socket
#endif
};

// Convert string (with IP address or hostname) to IP address:
// s (i): string with IPv4 address
// return value (o): IP address, 0 if error occured

static unsigned int ip_name2ip(const char* name)
{
	unsigned int ip;
	struct hostent* hent;
	
	// try if string contains IP address:
	
	if((ip = inet_addr(name)) != INADDR_NONE){
		return ntohl(ip);
	}

	// try if string contains hostname:

	hent = gethostbyname(name); 

	if(!hent){
		return 0;
	}

	return ntohl(((struct in_addr *)hent->h_addr)->s_addr);
}


// ---------------------------------------------------------------------------------------------------
// Handling UDP data:

// Initialize UDP socket:
// sock (o): socket number
// port (io): port number, 0 if to be chosen by the OS
// return value (o): 0 if ok, <0 if error occured

static int udp_init(void** sock, unsigned short* port)
{
	struct _ip_socket_struct* s;
	struct sockaddr_in addr;
#ifdef OS_UNIX
	socklen_t addrlen;
#endif
#ifdef OS_WIN
	int addrlen;
#endif

	s = (struct _ip_socket_struct *)malloc(sizeof(struct _ip_socket_struct));

	if(s == NULL){
		return -11;
	}

	// initialize socket dll (only Windows):

#ifdef OS_WIN
	{
		WORD vreq;
		WSADATA wsa;

		vreq = MAKEWORD(2, 0);
		
		if(WSAStartup(vreq, &wsa) != 0){
			free(s);
			return -1;
		}
	}
#endif
        
	// create socket:

#ifdef OS_UNIX
	s->ossock = socket(PF_INET, SOCK_DGRAM, 0);
	
	if(s->ossock < 0){
		free(s);
		return -2;
	}
#endif
#ifdef OS_WIN
	s->ossock = socket(PF_INET, SOCK_DGRAM, 0);

	if(s->ossock == INVALID_SOCKET){
		WSACleanup();
		free(s);
		return -2;
	}
#endif

	// name socket:

	addr.sin_family = AF_INET;
	addr.sin_port = htons(*port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addrlen = sizeof(addr);

	if(bind(s->ossock, (struct sockaddr *)&addr, addrlen) < 0){
		udp_exit(s);
		return -3;
	}

	if(*port == 0){  // port number was chosen by the OS
		if(getsockname(s->ossock, (struct sockaddr *)&addr, &addrlen)){
			udp_exit(s);
			return -3;
		}

		*port = ntohs(addr.sin_port);
	}

	*sock = s;
   return 0;
}


// Deinitialize UDP socket:
// sock (i): socket number
// return value (o): 0 ok, -1 error

static int udp_exit(void* sock)
{
	int err;
	struct _ip_socket_struct* s = (struct _ip_socket_struct *)sock;

	if(sock == NULL){
		return 0;
	}

#ifdef OS_UNIX
	err = close(s->ossock);
#endif
#ifdef OS_WIN
	err = closesocket(s->ossock);
	WSACleanup();
#endif

	free(sock);

	if(err < 0){
		return -1;
	}

	return 0;
}


// Receive UDP data:
//   - tries to receive packets, as long as data are available
// sock (i): socket number
// buffer (o): buffer for UDP data
// maxlen (i): length of buffer
// tout_us (i): timeout in us (micro sec)
// return value (o): number of received bytes, <0 if error/timeout occured

static int udp_receive(const void* sock, void *buffer, int maxlen, int tout_us)
{
	int nbytes, err;
	fd_set set;
	struct timeval tout;
	struct _ip_socket_struct* s = (struct _ip_socket_struct *)sock;

	// waiting for data:

	FD_ZERO(&set);
	FD_SET(s->ossock, &set);

	tout.tv_sec = tout_us / 1000000;
	tout.tv_usec = tout_us % 1000000;

	switch((err = select(FD_SETSIZE, &set, NULL, NULL, &tout))){
		case 1:
			break;        // data available
		case 0:
			return -1;    // timeout
		default:
	      return -2;    // error
	}

	// receiving packet:

	while(1){

		// receive one packet:

		nbytes = recv(s->ossock, (char *)buffer, maxlen, 0);

		if(nbytes < 0){  // receive error
			return -3;
		}

		// check, if more data available: if so, receive another packet
		
		FD_ZERO(&set);
		FD_SET(s->ossock, &set);

		tout.tv_sec = 0;   // no timeout
		tout.tv_usec = 0;

		if(select(FD_SETSIZE, &set, NULL, NULL, &tout) != 1){
			
			// no more data available: check length of received packet and return
			
			if(nbytes >= maxlen){   // buffer overflow
      		return -4;
		   }

			return nbytes;
		}
	}
}


// ---------------------------------------------------------------------------------------------------
// Handling TCP data:

// Initialize client TCP socket:
// sock (o): socket number
// ip (i): ip address of TCP server
// port (i): port number of TCP server
// return value (o): 0 if ok, <0 if error occured

static int tcp_client_init(void** sock, unsigned int ip, unsigned short port)
{
	struct _ip_socket_struct* s;
	struct sockaddr_in addr;

	s = (struct _ip_socket_struct *)malloc(sizeof(struct _ip_socket_struct));

	if(s == NULL){
		return -11;
	}

	// initialize socket dll (only Windows):

#ifdef OS_WIN
	{
		WORD vreq;
		WSADATA wsa;

		vreq = MAKEWORD(2, 0);
		
		if(WSAStartup(vreq, &wsa) != 0){
			free(s);
			return -1;
		}
	}
#endif
        
	// create socket:

#ifdef OS_UNIX
	s->ossock = socket(PF_INET, SOCK_STREAM, 0);
	
	if(s->ossock < 0){
		free(s);
		return -2;
	}
#endif
#ifdef OS_WIN
	s->ossock = socket(PF_INET, SOCK_STREAM, 0);

	if(s->ossock == INVALID_SOCKET){
		WSACleanup();
		free(s);
		return -2;
	}
#endif

	// connect with server:

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);
	
	if((connect(s->ossock, (struct sockaddr *)&addr, (size_t )sizeof(addr)))){
		tcp_exit(s);
		return -3;
	}

	*sock = s;
	return 0;
}


// Deinitialize TCP socket:
// sock (i): socket number
// return value (o): 0 ok, -1 error

static int tcp_exit(void* sock)
{
	int err;
	struct _ip_socket_struct* s = (struct _ip_socket_struct *)sock;

	if(sock == NULL){
		return 0;
	}

#ifdef OS_UNIX
	err = close(s->ossock);
#endif
#ifdef OS_WIN
	err = closesocket(s->ossock);
	WSACleanup();
#endif

	free(sock);

	if(err < 0){
		return -1;
	}

	return 0;
}


// Receive TCP data:
// sock (i): socket number
// buffer (o): buffer for TCP data
// maxlen (i): length of buffer
// tout_us (i): timeout in us (micro sec)
// return value (o): number of received bytes, <0 if error/timeout occured, -9 broken connection

static int tcp_receive(const void* sock, void *buffer, int maxlen, int tout_us)
{
	int nbytes, err;
	fd_set set;
	struct timeval tout;
	struct _ip_socket_struct* s = (struct _ip_socket_struct *)sock;

	// waiting for data:

	FD_ZERO(&set);
	FD_SET(s->ossock, &set);

	tout.tv_sec = tout_us / 1000000;
	tout.tv_usec = tout_us % 1000000;

	switch((err = select(FD_SETSIZE, &set, NULL, NULL, &tout))){
		case 1:
			break;        // data available
		case 0:
			return -1;    // timeout
		default:
	      return -2;    // error
	}

	// receiving packet:

	nbytes = recv(s->ossock, (char *)buffer, maxlen, 0);

	if(nbytes == 0){  // broken connection
		return -9;
	}

	if(nbytes < 0){  // receive error
		return -3;
	}

	if(nbytes >= maxlen){   // buffer overflow
		return -4;
	}

	return nbytes;
}


// Send TCP data:
// sock (i): socket number
// buffer (i): buffer for TCP data
// len (i): length of buffer
// tout_us (i): timeout in us (micro sec)
// return value (o): 0 if ok, <0 if error/timeout occured

static int tcp_send(const void* sock, const void* buffer, int len, int tout_us)
{
	fd_set set;
	struct timeval tout;
	int nbytes, err;
	struct _ip_socket_struct* s = (struct _ip_socket_struct *)sock;

	// waiting to send data:

	FD_ZERO(&set);
	FD_SET(s->ossock, &set);

	tout.tv_sec = tout_us / 1000000;
	tout.tv_usec = tout_us % 1000000;

	switch((err = select(FD_SETSIZE, NULL, &set, NULL, &tout))){
		case 1:
			break;
		case 0:
			return -1;    // timeout
		default:
	      return -2;    // error
	}

	// sending data:

	nbytes = send(s->ossock, (const char* )buffer, len, 0);

	if(nbytes < len){  // send error
      return -3;
   }

	return 0;
}


