/*
 * @filename 		PinWrapperWinAPI.h
 * @version			1.0, November 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Abstract class for creating new wrappers for Windows APIs to be spoofed by Pin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "pin.H"
#include <iostream>
#include <wchar.h>

#include "common.h"

// Detection wrappers
#include "VirtualboxDetectionAPIWrapper.h"
#include "VMWareDetectionAPIWrapper.h"
#include "VirtualPCDetectionAPIWrapper.h"
#include "SandboxDetectionAPIWrapper.h"
#include "DebuggerDetectionAPIWrapper.h"
#include "PinDBIDetectionAPIWrapper.h"
#include "PinVMShieldDetection.h"

#include "PinWrapperWindows.h"

namespace WINDOWS{
	#include <windows.h>
}

#define MAX_LENGTH_FUNCNAME 1024

class PinWrapperWinAPI: public PinWrapperWindows
{
private:
//	static vector<DetectionAPIWrapper*> createDetectionAvoidanceVector();

protected:
	static vector<DetectionWrapper*> createDetectionAvoidanceVector();

	char funcName[MAX_LENGTH_FUNCNAME];
	// These functions may be redefined by children, if needed
	virtual RTN FindRTNByNameA(IMG img){
		char *tmp = _strdup(this -> funcName);
		RTN rtn = RTN_FindByName(img, strcat(tmp, "A"));
		free(tmp);
		return rtn;
	};
	virtual RTN FindRTNByNameW(IMG img){
		char *tmp = _strdup(this -> funcName);
		RTN rtn = RTN_FindByName(img, strcat(tmp, "W"));
		free(tmp);
		return rtn;
	}
	virtual PROTO GetPrototypeFunctionA()
	{
		char *tmp = _strdup(this -> funcName);
		PROTO proto = GetPrototypeFunction(strcat(tmp, "A"));
		free(tmp);
		return proto;
	}
	virtual PROTO GetPrototypeFunctionW()
	{
		char *tmp = _strdup(this -> funcName);
		PROTO proto = GetPrototypeFunction(strcat(tmp, "W"));
		free(tmp);
		return proto;
	}

	// Abstract methods
	virtual PROTO GetPrototypeFunction(char *funcName) = 0;
	virtual void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode) = 0;

public:
	PinWrapperWinAPI():PinWrapperWindows()
	{
	}
	~PinWrapperWinAPI();
	void ReplaceWinAPI(IMG img);
	static bool checkDetection(CONTEXT *ctx, char *fName, char *s, bool isUnicode);

	static void setSandboxBehaviour(bool sandboxBehaviour);
	static bool isSandboxBehaviour();
};

static bool sBehaviour = false;

void PinWrapperWinAPI::setSandboxBehaviour(bool sandboxBehaviour)
{
	sBehaviour = sandboxBehaviour;
}

bool PinWrapperWinAPI::isSandboxBehaviour()
{
	return sBehaviour;
}

/**
   Function used to create a vector with all detections that need to be checked somehow. See specific classes for more details.
   @return a vector with all detections that need to be checked
 */
vector<DetectionWrapper*> PinWrapperWinAPI::createDetectionAvoidanceVector()
{
	vector<DetectionWrapper*>  v;
	// Add detection for VirtualBox
	v.push_back(new VirtualboxDetectionAPIWrapper());
	// Add detection for VMWare
	v.push_back(new VMWareDetectionAPIWrapper());
	// Add detection for VMWare
	v.push_back(new VirtualPCDetectionAPIWrapper());
	// Add detection for sandbox enviroments
	v.push_back(new SandboxDetectionAPIWrapper());
	// Add detection for debuggers
	v.push_back(new DebuggerDetectionAPIWrapper());
	// Add detection for Pin SBI
	v.push_back(new PinDBIDetectionAPIWrapper());
	// Add detection for PinVMShield
	v.push_back(new PinVMShieldDetection());

	return v;
}

/**
   Function used to check a string comparing it with different values related to detection of VMs/sandbox
   @param s the string to be checked
   @return a boolean indicating whether s is presumably a string related to some kind of detection
 */
/*bool PinWrapperWinAPI::checkDetection(WINDOWS::LPCWSTR s)
{
	vector<DetectionWrapper*> vDetection = createDetectionAvoidanceVector();
	bool isTryingDetection = false;
	//WINDOWS::LPCWSTR wstrLower = _wcslwr((WINDOWS::LPCWSTR *)s);

	// And iterates over the components...
	for(vector<DetectionWrapper*>::iterator it = vDetection.begin(); it != vDetection.end() && !isTryingDetection; it++)
	{
		isTryingDetection =( *it) -> checkIfTriesDetection(s);
	}
	
	return isTryingDetection;
}*/

/**
   Function used to check a string comparing it with different values related to detection of VMs/sandbox
   @param ctx
   @param s the string to be checked
   @param isUnicode
   @return a boolean indicating whether s is presumably a string related to some kind of detection
 */
bool PinWrapperWinAPI::checkDetection(CONTEXT *ctx, char *fName, char *s, bool isUnicode)
{
	vector<DetectionWrapper*> vDetection = createDetectionAvoidanceVector();
	bool isTryingDetection = false;
	char *str = s;

	if(isUnicode) // Check if unicode, if so then convert the unicode string to char*
	{
		//XXX I know, we should do it at the reverse (ASCII to Unicode). But, who matters? Normally, Windows internals
		// are ASCII. And, besides, this is motivated because otherwise the tool does not work fine 
		// integrated with Cuckoo Sandbox -- it would be fantastic to know the reason behind this
		int len = 4096;
		char *ss = new char[len];
		sprintf(ss, "%ls\0",s);
		//printMessage(ss);

		str = ss;
	}
	
	// If working as sandbox, log to file
	if(sBehaviour)
	{
		char aux[2048];
		sprintf(aux, "[LOG]: %x, %s, [%s]\n", (ADDRINT)PIN_GetContextReg(ctx, REG_INST_PTR), fName, str);
		printMessage(aux);
	}


	// And iterates over the components...
	for(vector<DetectionWrapper*>::iterator it = vDetection.begin(); it != vDetection.end() && !isTryingDetection; it++)
	{
		isTryingDetection =((DetectionAPIWrapper*)( *it)) -> triesDetection(str);
	}
	
	return isTryingDetection;
}

void PinWrapperWinAPI::ReplaceWinAPI(IMG img)
{
	RTN rtn = FindRTNByNameA(img);
	if(RTN_Valid(rtn))
	{
		PROTO proto = GetPrototypeFunctionA();
		ReplaceFunctionSignature(rtn, proto, false);
		PROTO_Free(proto);
	}
	rtn = FindRTNByNameW(img);
	if(RTN_Valid(rtn))
	{
		PROTO proto = GetPrototypeFunctionW();
		ReplaceFunctionSignature(rtn, proto, true);
		PROTO_Free(proto);
	}
}
