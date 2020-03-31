/*
 * @filename 		WrapperGetModuleHandle.h
 * @version			1.0, December 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap GetModuleHandleA/GetModuleHandleW Windows APIs
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

class WrapperGetModuleHandle: public PinWrapperWinAPI
{
	private:
		/**
		 Wrapper of GetModuleHandleA/GetModuleHandleW Windows APIs. Spoofs the return if needed
				before returning to the caller
		 @param origFunc a pointer to the address of the original function (GetModuleHandleA/GetModuleHandleW)
		 @param lpModuleName the name of the loaded module (either a .dll or .exe file). If the file name extension is omitted, the default library extension .dll is appended
		 @param ctx a pointer to a CONTEXT Pin structure that stores the architectural state of the processor
		 @return a handle of the loaded module, if any
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/ms683199(v=vs.85).aspx
		 */
		static WINDOWS::HMODULE myGetModuleHandle(AFUNPTR orig, 
										WINDOWS::LPCTSTR lpModuleName, CONTEXT *ctx,
										bool isUnicode)
		{
			WINDOWS::HMODULE retVal;
			PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
									CALLINGSTD_STDCALL, orig,
									PIN_PARG(WINDOWS::HMODULE), &retVal,
									PIN_PARG(WINDOWS::LPCTSTR), lpModuleName,
									PIN_PARG_END()
									);
			bool detectionVMs = checkDetection(ctx, "GetModuleHandle", (char *)lpModuleName, isUnicode);

			if(detectionVMs)
			{
				printMessage("\t-> [GMH] Asked for a dubious module\n");
				return NULL;
			}

			return retVal;
		}
	protected:
		PROTO GetPrototypeFunction(char *funcName)
		{
			return PROTO_Allocate( 
					PIN_PARG(WINDOWS::HMODULE), 
					CALLINGSTD_STDCALL, funcName,
					PIN_PARG(WINDOWS::LPCTSTR),
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperGetModuleHandle::myGetModuleHandle,
					IARG_PROTOTYPE, proto,
					IARG_ORIG_FUNCPTR,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_CONTEXT,
					IARG_BOOL, isUnicode,
					IARG_END
					);
		}

	public:
		WrapperGetModuleHandle():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "GetModuleHandle");
		}

		~WrapperGetModuleHandle(){};
};