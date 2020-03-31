/*
 * @filename 		WrapperGetModuleHandleEx.h
 * @version			1.0, December 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap GetModuleHandleExA/GetModuleHandleExW Windows APIs
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

class WrapperGetModuleHandleEx: public PinWrapperWinAPI
{
	private:
		/**
		  Wrapper of GetModuleHandleExA/GetModuleHandleExW Windows APIs. Spoofs the return if needed
			before returning to the caller
		 @param origFunc a pointer to the address of the original function (GetModuleHandleExA/GetModuleHandleExW)
		 @param dwFlags this parameter can be zero or one or more of the following values: 
				GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS (0x00000004), meaning that the lpModuleName parameter is an address in the module;
				GET_MODULE_HANDLE_EX_FLAG_PIN (0x00000001), meaning that the module stays loaded until the process is terminated, no matter how many times FreeLibrary is called;
				GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT (0x00000002), meaning that the reference count for the module is not incremente
		 @param lpModuleName the name of the loaded module (either a .dll or .exe file), or an address in the module (if dwFlags is GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS)
		 @param phModule a handle to the specified module. If the function fails, this parameter is NULL
		 @param ctx a pointer to a CONTEXT Pin structure that stores the architectural state of the processor
		 @return a handle of the loaded module, if any
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/ms683200(v=vs.85).aspx
		 */
		static WINDOWS::BOOL myGetModuleHandleEx(AFUNPTR orig, 
											 WINDOWS::DWORD dwFlags, WINDOWS::LPCTSTR lpModuleName, 
											 WINDOWS::HMODULE *phModule, CONTEXT *ctx,
											 bool isUnicode)
		{
			WINDOWS::BOOL retVal;
			PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
									CALLINGSTD_STDCALL, orig,
									PIN_PARG(WINDOWS::BOOL), &retVal,
									PIN_PARG(WINDOWS::DWORD), dwFlags,
									PIN_PARG(WINDOWS::LPCTSTR), lpModuleName,
									PIN_PARG(WINDOWS::HMODULE), phModule,
									PIN_PARG_END()
									);
			
			bool detectionVMs = checkDetection(ctx, "GetModuleHandleEx", (char *)lpModuleName, isUnicode);

			if(detectionVMs)
			{
				printMessage("\t-> [GMHE] Asked for a a dubious module\n");
				phModule = NULL;
				return 0;
			}

			return retVal;
		}
	protected:
		PROTO GetPrototypeFunction(char *funcName)
		{
			return PROTO_Allocate( 
					PIN_PARG(WINDOWS::BOOL), 
					CALLINGSTD_STDCALL, funcName,
					PIN_PARG(WINDOWS::DWORD),
					PIN_PARG(WINDOWS::LPCTSTR),
					PIN_PARG(WINDOWS::HMODULE),
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperGetModuleHandleEx::myGetModuleHandleEx,
					IARG_PROTOTYPE, proto,
					IARG_ORIG_FUNCPTR,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
					IARG_CONTEXT,
					IARG_BOOL, isUnicode,
					IARG_END
					);
		}

	public:
		WrapperGetModuleHandleEx():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "GetModuleHandleEx");
		}

		~WrapperGetModuleHandleEx(){};
};