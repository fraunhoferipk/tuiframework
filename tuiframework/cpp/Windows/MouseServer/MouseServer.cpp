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

 /**
  *
 *  \author Oliver Belaifa
 *  \date 2013
 */

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <CommCtrl.h>
#include <tchar.h>
#include "resource.h"


#include <tuiframework/sockets/UDPSenderWinSock2.h>
#include <tuiframework/core/HostMsg.h>
#include <tuiframework/core/HostMsgQueue.h>
#include <tuiframework/core/HostAddress.h>
#include <tuilibs/tuitypes/common/MouseData.h>

#include <cstdio>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

#pragma comment(linker, \
	"\"/manifestdependency:type='Win32' "\
	"name='Microsoft.Windows.Common-Controls' "\
	"version='6.0.0.0' "\
	"processorArchitecture='*' "\
	"publicKeyToken='6595b64144ccf1df' "\
	"language='*'\"")

#pragma comment(lib, "ComCtl32.lib")

static BOOL configEnabled = FALSE;
static RAWINPUTDEVICE rid[5];
static UINT rawInputBufferSize = 0;
static BYTE * rawInputBuffer = 0;
static RAWINPUT * rawInput = 0;
static int lastControlID = 0;
static HANDLE lastHDevice = 0;

using namespace std;

static map<HANDLE, string> hDeviceToDeviceNameMap;
static map<string, HANDLE> deviceNameToHDeviceMap;
static map<int, int> idToControlIDMap;
static map<HANDLE, int> hDeviceToIDMap;

static tuiframework::HostMsgQueue hostMsgQueue;
static tuiframework::UDPSenderWinSock2 senderSocket(hostMsgQueue);
static tuiframework::HostAddress hostAddress;
static bool serverIsRunning = false;
static stringstream ss;

// saves current settings to the config file MouseServer.cfg
void store(HWND hDlg) {
	map<string, int> deviceNameToIDMap;
	{
		map<HANDLE, int>::const_iterator i = hDeviceToIDMap.begin();
		map<HANDLE, int>::const_iterator e = hDeviceToIDMap.end();
		while (i != e) {
			map<HANDLE, string>::const_iterator iter = hDeviceToDeviceNameMap.find((*i).first);
			if (iter != hDeviceToDeviceNameMap.end()) {
				deviceNameToIDMap[(*iter).second] = (*i).second;
			} else {
				MessageBox(hDlg, "Unknown Error", "Mouse Server", MB_ICONERROR | MB_OK);
			}
			++i;
		}
	}

	fstream fs;
	fs.open("MouseServer.cfg", fstream::out);
	if (fs.fail()) {
		MessageBox(hDlg, "Failed to write the config file", "Mouse Server", MB_ICONERROR | MB_OK);
		return;
	}

	fs << deviceNameToIDMap.size() << endl;
	{
		map<string, int>::iterator i = deviceNameToIDMap.begin();
		map<string, int>::iterator e = deviceNameToIDMap.end();
		while (i != e) {
			fs << (*i).first << " " << (*i).second << endl;
			++i;
		}
	}
	fs << hostAddress << endl;
	fs.close();
}

// reads the settings from the config file MouseServer.cfg
void restore(HWND hDlg) {
	fstream fs;
	fs.open("MouseServer.cfg", fstream::in);
	if (fs.fail()) {
		return;
	}

	int count;
	fs >> count;

	if (count > 4) {
		MessageBox(hDlg, "Failed to read the config file", "Mouse Server", MB_ICONERROR | MB_OK);
		fs.close();
		return;
	}

	map<string, int> deviceNameToIDMap;
	string key;
	int value;
	for (int i = 0; i < count; ++i) {
		fs >> key;
		fs >> value;
		deviceNameToIDMap[key] = value;
	}

	try {
		fs >> hostAddress;
	}
	catch (...) {
		MessageBox(hDlg, "Failed to read the config file", "Mouse Server", MB_ICONERROR | MB_OK);
	}
	fs.close();

	hDeviceToIDMap.clear();
	map<string, int>::iterator i = deviceNameToIDMap.begin();	
	map<string, int>::iterator e = deviceNameToIDMap.end();
	while (i != e) {
		map<string, HANDLE>::const_iterator iter = deviceNameToHDeviceMap.find((*i).first);
		if (iter != deviceNameToHDeviceMap.end()) {
			hDeviceToIDMap[(*iter).second] = (*i).second;
		} else {
			MessageBox(hDlg, (string("Device: ") + (*i).first + string(" not found!")).c_str(), "Mouse Server", MB_ICONERROR | MB_OK);
		}
		++i;
	}
}

// refreshes the view
static void refreshAllocation(HWND hDlg) {
	SetDlgItemText(hDlg, IDC_STATIC_MOUSE1, "");
	SetDlgItemText(hDlg, IDC_STATIC_MOUSE2, "");
	SetDlgItemText(hDlg, IDC_STATIC_MOUSE3, "");
	SetDlgItemText(hDlg, IDC_STATIC_MOUSE4, "");
	
	map<HANDLE, int>::iterator i = hDeviceToIDMap.begin();
	map<HANDLE, int>::iterator e = hDeviceToIDMap.end();
	while (i != e) {
		char name[512];
		UINT nameSize = 255;
		GetRawInputDeviceInfo((*i).first, RIDI_DEVICENAME, name, &nameSize);
		SetDlgItemText(hDlg, idToControlIDMap[(*i).second], name);
		++i;
	}
}

// updates the assignments as a function of the requested id for the assignment of the current input
static void updateAllocation(int id) {
	if ( ! rawInput) {
		return;
	}
	map<HANDLE, int>::iterator iter = hDeviceToIDMap.find(rawInput->header.hDevice);
	if (iter != hDeviceToIDMap.end()) {
		if ((*iter).second != id) {
			map<HANDLE, int>::iterator i = hDeviceToIDMap.begin();
			map<HANDLE, int>::iterator e = hDeviceToIDMap.end();
			while (i != e) {
				if ((*i).second == id) {
					hDeviceToIDMap.erase(i);
					i = hDeviceToIDMap.begin();
				}
				++i;
			}
			(*iter).second = id;
		} else {
			hDeviceToIDMap.erase(iter);
		}
	} else {
		hDeviceToIDMap[rawInput->header.hDevice] = id;
	}
}

// refreshes the view of the control with the id controlID
static void updateDeviceName(HWND hDlg, int controlID) {
	if ( ! rawInput) {
		return;
	}
	
	if (lastControlID == controlID && lastHDevice == rawInput->header.hDevice) {
		return;
	}
	lastControlID = controlID;
	lastHDevice = rawInput->header.hDevice;

	char name[512];
	UINT nameSize = 255;
	GetRawInputDeviceInfo(rawInput->header.hDevice, RIDI_DEVICENAME, name, &nameSize);
	SetDlgItemText(hDlg, controlID, name);
}


// registeres one keyboard and up to four mouse devices
static void registerRawInputDevices(HWND hDlg, DWORD dwFlags) {
	// 1 keyboard
	rid[0].usUsagePage = 1;
	rid[0].usUsage = 6;
	rid[0].dwFlags = dwFlags;
	rid[0].hwndTarget = hDlg;
		// 4 mouses
	for (int i = 1; i < 5; ++i) {
		rid[i].usUsagePage = 1;
		rid[i].usUsage = 2;
		rid[i].dwFlags = dwFlags;
		rid[i].hwndTarget = hDlg;
	}
	if (RegisterRawInputDevices(rid, 5, sizeof(RAWINPUTDEVICE)) == FALSE) {
		MessageBox(hDlg, "Registering input devices failed", "Mouse Server", MB_ICONERROR | MB_OK);
		PostQuitMessage(0);
	} 
}

// initializes the hDeviceToDeviceNameMap
void initHDeviceToDeviceNameMap() {
	UINT numDevices = 0;
	UINT size = sizeof(RAWINPUTDEVICELIST);
	GetRawInputDeviceList(0, &numDevices, size);
	RAWINPUTDEVICELIST * rl = new RAWINPUTDEVICELIST[numDevices];
	GetRawInputDeviceList(rl, &numDevices, size);

	hDeviceToDeviceNameMap.clear();
	deviceNameToHDeviceMap.clear();
	for (UINT i = 0; i < numDevices; ++i) {
		char name[512];
		UINT nameSize = 255;
		GetRawInputDeviceInfo(rl[i].hDevice, RIDI_DEVICENAME, name, &nameSize);
		hDeviceToDeviceNameMap[rl[i].hDevice] = name;
		deviceNameToHDeviceMap[name] = rl[i].hDevice;

		char buffer[512];
		sprintf_s(buffer, "%x -> %s\n", rl[i].hDevice, name);
		OutputDebugString(buffer);
	}

	delete []rl;
}

// enables or dispables (by parameter enable) the configuration controls
static void enableConfiguration(HWND hDlg, BOOL enable) {
	EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_MOUSE1), enable);
	EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_MOUSE2), enable);
	EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_MOUSE3), enable);
	EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_MOUSE4), enable);
	EnableWindow(GetDlgItem(hDlg, IDC_IPADDRESS1), enable);
	EnableWindow(GetDlgItem(hDlg, IDC_EDIT_PORT1), enable);

	EnableWindow(GetDlgItem(hDlg, ID_BUTTON_START_SERVER), ! enable);

	configEnabled = enable;
}

// updates the view the locked button
static void updateLockedButton(HWND hDlg) {
	SetDlgItemText(hDlg, IDC_BUTTON_LOCK,  configEnabled ? "Lock" : "Unlock");
}

// verifies the input of the port number whether it is within the range
static void checkPortNr(HWND hDlg) {
	char text[16];
	bool update = false;

	GetDlgItemText(hDlg, IDC_EDIT_PORT1, text, 16);
	int portNr = atoi(text);
	if (portNr > 65535) {
		portNr = 65535;
		update = true;
	}
	if (update) {
		sprintf_s(text, "%d", portNr);
		SetDlgItemText(hDlg, IDC_EDIT_PORT1, text);
	}
}

// refreshes the host address control
static void setHostAddressToControl(HWND hDlg) {
	HWND hWnd = GetDlgItem(hDlg, IDC_IPADDRESS1);
	SendMessage(hWnd, IPM_SETADDRESS, 0, (LPARAM)hostAddress.getIPAddress());

	char text[16];
	sprintf_s(text, "%d", hostAddress.getPortNr());
	SetDlgItemText(hDlg, IDC_EDIT_PORT1, text);
}

// stores the content of the host address control to hostAddress
static void setControlToHostAddress(HWND hDlg) {
	HWND hWnd = GetDlgItem(hDlg, IDC_IPADDRESS1);
	DWORD addr;
	SendMessage(hWnd, IPM_GETADDRESS, 0, (LPARAM)(LPDWORD)&addr);
	hostAddress.setIPAddress(addr);

	char text[16];
	GetDlgItemText(hDlg, IDC_EDIT_PORT1, text, 16);
	int portNr = atoi(text);
	hostAddress.setPortNr(portNr);
}

// initializes the idToControlIDMap
static void initIDToControlIDMap() {
	idToControlIDMap[1] = IDC_STATIC_MOUSE1;
	idToControlIDMap[2] = IDC_STATIC_MOUSE2;
	idToControlIDMap[3] = IDC_STATIC_MOUSE3;
	idToControlIDMap[4] = IDC_STATIC_MOUSE4;
}

// Used for cummulation. Stores the MouseData for each channel.
static map<int, MouseData> idToMouseDataMap;

// dispatches the MouseData of the id channel
static void dispatch(int id) {
	MouseData & md = idToMouseDataMap[id];
	md.setId(id);
	ss.str("");
	ss << md;
	string str = ss.str();
	tuiframework::HostMsg * msg = new tuiframework::HostMsg(hostAddress, str.c_str(), str.size() + 1);
	hostMsgQueue.push(msg);
	md = MouseData();
}

// dispatches all changed MouseData
static void dispatchAll() {
	map<int, MouseData>::iterator i = idToMouseDataMap.begin();
	map<int, MouseData>::iterator e = idToMouseDataMap.end();
	while (i != e) {
		if ((*i).second.hasChanged()) {
			dispatch((*i).first);
		}
		++i;
	}
}


// buttons + cumulated movement
static void dispatchMouseInput(int id) {
	MouseData & md = idToMouseDataMap[id];
	md.setDeltaX(md.getDeltaX() + rawInput->data.mouse.lLastX);
	md.setDeltaY(md.getDeltaY() + rawInput->data.mouse.lLastY);
	if (rawInput->data.mouse.usButtonFlags) {
		if (rawInput->data.mouse.usButtonFlags & 0x400) {
			md.setDeltaWheel(static_cast<short>(rawInput->data.mouse.usButtonData));
		}
		md.setButtonCode(rawInput->data.mouse.usButtonFlags & 0x3ff);
		dispatch(id);
	}
}


/*
// buttons + movement
static void dispatchMouseInput(int id) {
	MouseData md(id);
	if (rawInput->data.mouse.usButtonFlags & 0x400) {
		md.setDeltaWheel(static_cast<short>(rawInput->data.mouse.usButtonData));
	}
	md.setDeltaX(rawInput->data.mouse.lLastX);
	md.setDeltaY(rawInput->data.mouse.lLastY);
	md.setButtonCode(rawInput->data.mouse.usButtonFlags & 0x3ff);
	ss.str("");
	ss << md;
	string str = ss.str();
	tuiframework::HostMsg * msg = new tuiframework::HostMsg(hostAddress, str.c_str(), str.size() + 1);
	hostMsgQueue.push(msg);
}
*/



/*
// only buttons
static void dispatchMouseInput(int id) {
	MouseData md(id);
	if ( ! rawInput->data.mouse.usButtonFlags) {
		return;
	}
	if (rawInput->data.mouse.usButtonFlags & 0x400) {
		md.setDeltaWheel(rawInput->data.mouse.usButtonData);
	}
	md.setButtonCode(rawInput->data.mouse.usButtonFlags & 0x3ff);
	ss.str("");
	ss << md;
	string str = ss.str();
	tuiframework::HostMsg * msg = new tuiframework::HostMsg(hostAddress, str.c_str(), str.size() + 1);
	hostMsgQueue.push(msg);
}
*/

// windows event handler function for the main dialog
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			enableConfiguration(hDlg, FALSE);
			updateLockedButton(hDlg);
			registerRawInputDevices(hDlg, 0);
			initHDeviceToDeviceNameMap();
			initIDToControlIDMap();
			restore(hDlg);
			refreshAllocation(hDlg);
			setHostAddressToControl(hDlg);
			SetTimer(hDlg, 1, 15, 0);
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDCANCEL:
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			return TRUE;
		case IDC_IPADDRESS1:
			
			break;
		case IDC_BUTTON_LOCK:
			enableConfiguration(hDlg, ! configEnabled);
			updateLockedButton(hDlg);
			if ( ! configEnabled) {
				setControlToHostAddress(hDlg);
				store(hDlg);
			}
			break;
		case IDC_EDIT_PORT1: 
			if (HIWORD(wParam) == EN_CHANGE) {
				checkPortNr(hDlg);
			}
 			break;
		case IDC_BUTTON_MOUSE1:
			updateAllocation(1);
			refreshAllocation(hDlg);
			break;
		case IDC_BUTTON_MOUSE2:
			updateAllocation(2);
			refreshAllocation(hDlg);
			break;
		case IDC_BUTTON_MOUSE3:
			updateAllocation(3);
			refreshAllocation(hDlg);
			break;
		case IDC_BUTTON_MOUSE4:
			updateAllocation(4);
			refreshAllocation(hDlg);
			break;
		case ID_BUTTON_START_SERVER:
			registerRawInputDevices(hDlg, RIDEV_NOLEGACY | RIDEV_CAPTUREMOUSE);
			//registerRawInputDevices(hDlg, 0);
			SetDlgItemText(hDlg, IDC_STATIC1,  "Mouse Server started, press 's' to stop and release the capture");
			senderSocket.create();
			serverIsRunning = true;
			break;
		}
		break;
	case WM_INPUT:
		if (rawInputBuffer == 0) {
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &rawInputBufferSize, sizeof(RAWINPUTHEADER));
			rawInputBuffer = new BYTE[rawInputBufferSize];
			rawInput = (RAWINPUT *)rawInputBuffer;
		}
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, (LPVOID)rawInputBuffer, &rawInputBufferSize, sizeof(RAWINPUTHEADER));
		if (rawInput->header.dwType == RIM_TYPEMOUSE) {
			updateDeviceName(hDlg, IDC_STATIC_CURRENT_MOUSE);
			if (serverIsRunning) {
				map<HANDLE, int>::iterator iter = hDeviceToIDMap.find(rawInput->header.hDevice);
				if (iter != hDeviceToIDMap.end()) {
					dispatchMouseInput((*iter).second);
				}
			}
		} else if (rawInput->header.dwType == RIM_TYPEKEYBOARD) {
			if (rawInput->data.keyboard.VKey == 'S') {
				registerRawInputDevices(hDlg, 0);
				SetDlgItemText(hDlg, IDC_STATIC1,  "");
				serverIsRunning = false;
				senderSocket.cancel();
				senderSocket.cleanup();
			}
		}
		break;
	case WM_TIMER:
		dispatchAll();
		break;
	case WM_CLOSE:
		if(MessageBox(hDlg, TEXT("Close the program?"), TEXT("Mouse Server"),
			MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			DestroyWindow(hDlg);
		}
		return TRUE;

	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	}

	return FALSE;
}


int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow)
{
	HWND hDlg;
	MSG msg;
	BOOL ret;

	InitCommonControls();
	hDlg = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc, 0);
	ShowWindow(hDlg, nCmdShow);

	while((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
		if(ret == -1)
			return -1;

		if(!IsDialogMessage(hDlg, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}
