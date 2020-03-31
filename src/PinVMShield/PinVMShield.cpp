/*
 * @filename 		PinVMShield.cpp
 * @version			1.0, November 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Implements a pintool to avoid sandbox and virtual machine detection. 
 *		It is intended to be integrated with Cuckoo Sandbox for performing malware analysis tasks.
 *      Check for its last version at BitBucket: https://bitbucket.org/rjrodriguez/pinvmshield
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

#include "pin.H"
#include <stdio.h>
#include <vector>

#include "PinWrapperWinAPI.h"

// APIs
#include "WrapperGetUserName.h"
#include "WrapperGetUserNameEx.h"
#include "WrapperRegQueryValue.h"
#include "WrapperRegQueryValueEx.h"
#include "WrapperRegOpenKey.h"
#include "WrapperRegOpenKeyEx.h"
#include "WrapperGetModuleHandle.h"
#include "WrapperGetModuleHandleEx.h"
#include "WrapperGetFileAttributes.h"
#include "WrapperProcess32FirstAndNext.h"
#include "WrapperFindWindow.h"
#include "WrapperFindWindowEx.h"
#include "WrapperCreateFile.h"
#include "WrapperCreateNamedPipe.h"
#include "WrapperGetCursorPos.h"
#include "WrapperCallNamedPipe.h"

/**
 Called by Pin, fakes the APIs. Check above code
 @param img an IMG represents all the data structures corresponding to a binary (executable or library)
 @param v a pointer
 */
VOID Image(IMG img, VOID *v)
{
	vector<PinWrapperWinAPI*>  vWinAPIs;

	// Fill vector properly...
	vWinAPIs.push_back(new WrapperGetUserName()); // API GetUserNameA/W
	vWinAPIs.push_back(new WrapperGetUserNameEx()); // API GetUserNameExA/W
	vWinAPIs.push_back(new WrapperRegQueryValue()); // API RegQueryValueA/W
	vWinAPIs.push_back(new WrapperRegQueryValueEx()); // API RegQueryValueExA/W
	vWinAPIs.push_back(new WrapperRegOpenKey()); // API RegOpenKeyA/W
	vWinAPIs.push_back(new WrapperRegOpenKeyEx()); // API RegOpenKeyExA/W
	vWinAPIs.push_back(new WrapperGetModuleHandle()); // API GetModuleHandleA/W
	vWinAPIs.push_back(new WrapperGetModuleHandleEx()); // API GetModuleHandleExA/W
	vWinAPIs.push_back(new WrapperGetFileAttributes()); // API GetFileAttributesA/W
	//XXX This is not working, needs to be revised...
	//vWinAPIs.push_back(new WrapperProcess32FirstAndNext()); // API Process32First / Process32Next
	vWinAPIs.push_back(new WrapperFindWindow()); // API FindWindowA/W
	vWinAPIs.push_back(new WrapperFindWindowEx()); // API FindWindowExA/W
	vWinAPIs.push_back(new WrapperCreateFile()); // API CreateFileA/W
	vWinAPIs.push_back(new WrapperCreateNamedPipe()); // API CreateNamedPipeA/W
	vWinAPIs.push_back(new WrapperGetCursorPos()); // API GetCursorPos
	vWinAPIs.push_back(new WrapperCallNamedPipe()); // API CallNamedPipe

	// And iterates over the components...
	for(vector<PinWrapperWinAPI*>::iterator it = vWinAPIs.begin(); it != vWinAPIs.end(); it++)
	{
		(*it) -> ReplaceWinAPI(img);
	}
}


/**
 Last function of pintool that will be executed
 */
VOID Fini(INT32 code, VOID *v)
{
	fprintf(stdout, "Exiting...\n");
	if(logFile != NULL)
		fclose(logFile);
}

/**
 Main procedure of pintool. Not very interested to comment if you are related to Pintools development
 */
int main(int argc, char *argv[])
{
	// Initialize pin & symbol manager
    PIN_InitSymbols();
    PIN_Init(argc,argv);

    // Register Image to be called to instrument functions.
    IMG_AddInstrumentFunction(Image, 0);

    PIN_StartProgram(); // Never returns
    
    return 0;
}
