/*
 * @filename 		WrapperProcess32FirstAndNext.h
 * @version			1.0, December 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap Process32First and Process32Next  Windows APIs
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

#include "PinWrapperWinAPI.h"

namespace WINDOWS{
 	#include <tlhelp32.h>
}

class WrapperProcess32FirstAndNext: public PinWrapperWinAPI
{
	private:
		/**
		 Spoofs the process name
		 @param lppe a pointer to a LPPROCESSENTRY32 structure
		   XXX: It assumes lppe is NOT null, and does no checking, so prone to error...
		   This could be done better to modify only certain process names
		  */
		 static void changeProcessName(WINDOWS::LPPROCESSENTRY32 lppe)
		 {
 			char s[4096];
			
			sprintf(s, "\t-> [P32F/P32N] Parsing process name (%s)\n", lppe -> szExeFile);
			printMessage(s);
			int len = strlen((const char*)lppe -> szExeFile);
			for(int i = 0; i < len; i++)
				lppe -> szExeFile[i] += i;
		 }

		/**
		 Wrapper of Process32First and Process32Next Windows APIs. Executes a procedure to change the name of the return process 
				before returning to the caller
		 @param origFunc a pointer to the address of the original function (Process32First or Process32Next)
		 @param hSnapshot a handle to the snapshot returned from a previous call to the CreateToolhelp32Snapshot function
		 @param lppe a pointer to a PROCESSENTRY32 structure. It contains process information such as the name of the executable file, 
				the process identifier, and the process identifier of the parent process.
		 @param ctx a pointer to a CONTEXT Pin structure that stores the architectural state of the processor
		 @return a boolean value, the same value that returns the original Windows API Process32First/Process32Next. 
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/ms684834(v=vs.85).aspx 
											(or http://msdn.microsoft.com/en-us/library/windows/desktop/ms684836(v=vs.85).aspx)
		 */
		static WINDOWS::BOOL myProcess32FirstAndNext(AFUNPTR origFunc, 
										WINDOWS::HANDLE hSnapshot, WINDOWS::LPPROCESSENTRY32 lppe, 
										CONTEXT *ctx,
										bool isUnicode)
		{
			WINDOWS::BOOL retVal;
			PIN_CallApplicationFunction(ctx, PIN_ThreadId(), 
									CALLINGSTD_STDCALL, origFunc, 
									PIN_PARG(BOOL), &retVal,
									PIN_PARG(WINDOWS::HANDLE), hSnapshot, 
									PIN_PARG(WINDOWS::LPPROCESSENTRY32), lppe,
									PIN_PARG_END());

			// Ok, now we change the name of the process... Just in case malware wants to check somehow...
			bool detectionVMs = checkDetection(ctx, "Process32First/Next", (char *) lppe -> szExeFile, isUnicode);

			if(detectionVMs)
			{
				//XXX When detected, do something
				printMessage("\t-> [P32F/P32N] Asking for a dubious process name\n");
				//changeProcessName(lppe);
			} 

			return retVal;
		}
	protected:
		PROTO GetPrototypeFunction(char *funcName)
		{
			return PROTO_Allocate( 
					PIN_PARG(BOOL), CALLINGSTD_STDCALL,
					funcName,
					PIN_PARG(WINDOWS::HANDLE), 
					PIN_PARG(WINDOWS::LPPROCESSENTRY32),
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperProcess32FirstAndNext:: myProcess32FirstAndNext,
					IARG_PROTOTYPE, proto,
					IARG_ORIG_FUNCPTR,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
					IARG_CONTEXT,
					IARG_BOOL, isUnicode,
					IARG_END
			);
		}

		RTN FindRTNByNameA(IMG img){
			RTN rtn = RTN_FindByName(img, "Process32First");
			return rtn;
		}

		RTN FindRTNByNameW(IMG img){
			RTN rtn = RTN_FindByName(img, "Process32Next");
			return rtn;
		}

		virtual PROTO GetPrototypeFunctionA()
		{
			PROTO proto = GetPrototypeFunction("Process32First");
			return proto;
		}
		virtual PROTO GetPrototypeFunctionW()
		{
			PROTO proto = GetPrototypeFunction("Process32Next");
			return proto;
		}

	public:
		WrapperProcess32FirstAndNext():PinWrapperWinAPI()
		{
		
		}

		~WrapperProcess32FirstAndNext(){};
};