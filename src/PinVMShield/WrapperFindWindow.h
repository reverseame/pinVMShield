/*
 * @filename 		WrapperFindWindow.h
 * @version			1.0, December 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap FindWindow Windows API
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

class WrapperFindWindow: public PinWrapperWinAPI
{
	private:
		/**
		 Wrapper of FindWindow Windows APIs. Spoofs the return if needed
				before returning to the caller
		 @param lpClassName the class name or a class atom created by a previous call to the RegisterClass or RegisterClassEx function
		 @param lpWindowName the window name (the window's title)
		 @param ctx a pointer to a CONTEXT Pin structure that stores the architectural state of the processor
		 @return a window handle, the same value that original that returns Windows API GetFileAttributes
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/ms633499(v=vs.85).aspx
		 */
		static WINDOWS::HWND myFindWindow (AFUNPTR orig, 
										 WINDOWS::LPCTSTR lpClassName, 
										 WINDOWS::LPCTSTR lpWindowName,
										 CONTEXT *ctx,
										 bool isUnicode)
		{
			WINDOWS::HWND retVal;
			PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
									CALLINGSTD_STDCALL, orig,
									PIN_PARG(WINDOWS::HWND), &retVal,
									PIN_PARG(WINDOWS::LPCTSTR), lpClassName,
									PIN_PARG(WINDOWS::LPCTSTR), lpWindowName,
									PIN_PARG_END()
									);

			bool detectionVMs = checkDetection(ctx, "FindWindow", (char *)lpClassName, isUnicode);

			if(detectionVMs)
			{
				printMessage("\t-> [FW] Asked for a dubious window handle\n");
				return NULL;
			}
			else
				return retVal;
		}
	protected:
		PROTO GetPrototypeFunction(char *funcName)
		{
			return PROTO_Allocate( 
					PIN_PARG(WINDOWS::LONG), 
					CALLINGSTD_STDCALL, funcName,
					PIN_PARG(WINDOWS::LPCTSTR),
					PIN_PARG(WINDOWS::LPCTSTR),
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperFindWindow::myFindWindow,
					IARG_PROTOTYPE, proto,
					IARG_ORIG_FUNCPTR,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0, // _In_opt_  LPCTSTR lpClassName,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 1, // _In_opt_  LPCTSTR lpWindowName
					IARG_CONTEXT,
					IARG_BOOL, isUnicode,
					IARG_END
					);
		}

	public:
		WrapperFindWindow():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "FindWindow");
		}

		~WrapperFindWindow(){};
};