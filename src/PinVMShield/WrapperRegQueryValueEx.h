/*
 * @filename 		WrapperRegQueryValueEx.h
 * @version			1.0, December 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap RegQueryValueExA/RegQueryValueExW Windows APIs
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

class WrapperRegQueryValueEx: public PinWrapperWinAPI
{
	private:
		/**
		 Wrapper of RegQueryValueExA/RegQueryValueExW Windows APIs. Spoofs the return if needed
				before returning to the caller
		 @param origFunc a pointer to the address of the original function (RegQueryValueExA/RegQueryValueExW)
		 @param hKey a handle to an open registry key
		 @param lpValueName the name of the registry value
		 @param lpReserved this parameter is reserved and must be NULL
		 @param lpType a pointer to a variable that receives a code indicating the type of data stored in the specified value
		 @param lpData a pointer to a buffer that receives the value's data
		 @param ctx a pointer to a CONTEXT Pin structure that stores the architectural state of the processor
		 @return a long value, the same value that original that returns Windows API RegQueryValueExA/RegQueryValueExW
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/ms724911(v=vs.85).aspx
		 */
		static WINDOWS::LONG myRegQueryValueEx(AFUNPTR orig, WINDOWS::HKEY hKey, 
										 WINDOWS::LPCTSTR lpValueName, WINDOWS::LPDWORD lpReserved, 
										 WINDOWS::LPDWORD lpType, WINDOWS::LPBYTE lpData, 
										 WINDOWS::LPDWORD lpcbData, CONTEXT *ctx,
										 bool isUnicode)
		{
			WINDOWS::LONG retVal;
			PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
									CALLINGSTD_STDCALL, orig,
									PIN_PARG(WINDOWS::LONG), &retVal,
									PIN_PARG(WINDOWS::HKEY), hKey,
									PIN_PARG(WINDOWS::LPCTSTR), lpValueName,
									PIN_PARG(WINDOWS::LPDWORD), lpReserved,
									PIN_PARG(WINDOWS::LPDWORD), lpType,
									PIN_PARG(WINDOWS::LPBYTE), lpData,
									PIN_PARG(WINDOWS::LPDWORD), lpcbData,
									PIN_PARG_END()
									);
			bool detectionVMs = checkDetection(ctx, "RegQueryValueEx", (char *)lpData, isUnicode);

			if(detectionVMs)
			{
				printMessage("\t-> [RegQVE] Parsing a dubious registry data value\n");
				return -1; // ERROR
			}

			return retVal;
		}
	protected:
		PROTO GetPrototypeFunction(char *funcName)
		{
			return PROTO_Allocate( 
					PIN_PARG(WINDOWS::LONG), 
					CALLINGSTD_STDCALL, funcName,
					PIN_PARG(WINDOWS::HKEY),
					PIN_PARG(WINDOWS::LPCTSTR), 
					PIN_PARG(WINDOWS::LPDWORD),
					PIN_PARG(WINDOWS::LPDWORD),
					PIN_PARG(WINDOWS::LPBYTE),
					PIN_PARG(WINDOWS::LPDWORD),
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperRegQueryValueEx::myRegQueryValueEx,
					IARG_PROTOTYPE, proto,
					IARG_ORIG_FUNCPTR,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 3,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 4,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 5,
					IARG_CONTEXT,
					IARG_BOOL, isUnicode,
					IARG_END
					);
		}

	public:
		WrapperRegQueryValueEx():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "RegQueryValueEx");
		}

		~WrapperRegQueryValueEx(){};
};