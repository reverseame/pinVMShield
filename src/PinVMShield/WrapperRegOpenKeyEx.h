/*
 * @filename 		WrapperRegOpenKeyEx.h
 * @version			1.0, December 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap RegOpenKeyExA/RegOpenKeyExW Windows APIs
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

#define SECURITY_WIN32 // Imposed by security.h

namespace WINDOWS{
	#include <security.h>
}


class WrapperRegOpenKeyEx: public PinWrapperWinAPI
{
	private:
			/**
			 Wrapper of RegOpenKeyExA/RegOpenKeyExW Windows APIs. Spoofs the return if needed
					before returning to the caller 
			 @param origFunc a pointer to the address of the original function (RegOpenKeyExA/RegOpenKeyExW)
			 @param hKey a handle to an open registry key
			 @param lpSubKey the name of the registry subkey to be opened
			 @param ulOptions this parameter is reserved and must be zero
			 @param samDesired a mask that specifies the desired access rights to the key to be opened
			 @param phkResult a pointer to a variable that receives a handle to the opened key
			 @param ctx a pointer to a CONTEXT Pin structure that stores the architectural state of the processor
			 @return a long value, the same value that original that returns Windows API RegOpenKeyExA/RegOpenKeyExW
					For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/ms724897(v=vs.85).aspx
			 */
			static WINDOWS::LONG myRegOpenKeyEx(AFUNPTR orig, WINDOWS::HKEY hKey, 
											 WINDOWS::LPCTSTR lpSubKey, WINDOWS::DWORD ulOptions, 
											 WINDOWS::REGSAM samDesired, WINDOWS::PHKEY phkResult,
											 CONTEXT *ctx,
											 bool isUnicode)
			{
				WINDOWS::LONG retVal;
				PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
										CALLINGSTD_STDCALL, orig,
										PIN_PARG(long), &retVal,
										PIN_PARG(WINDOWS::HKEY), hKey,
										PIN_PARG(WINDOWS::LPCTSTR), lpSubKey,
										PIN_PARG(WINDOWS::DWORD), ulOptions,
										PIN_PARG(WINDOWS::REGSAM), samDesired,
										PIN_PARG(WINDOWS::PHKEY), phkResult,
										PIN_PARG_END()
										);

				bool detectionVMs = checkDetection(ctx, "RegOpenKeyEx", (char *) lpSubKey, isUnicode);

				if(detectionVMs)
				{
					printMessage("\t-> [RegOKE] Parsing a dubious registry key\n");
					return -1;
				}
				else
						return retVal;
			}


	protected:
		PROTO GetPrototypeFunction(char *funcName)
		{
			return PROTO_Allocate( 
					PIN_PARG(int), 
					CALLINGSTD_STDCALL, funcName,
					PIN_PARG(WINDOWS::HKEY), // _In_        HKEY hKey
					PIN_PARG(WINDOWS::LPCTSTR), //  _In_opt_    LPCTSTR lpSubKey
					PIN_PARG(WINDOWS::DWORD), // _Reserved_  DWORD ulOptions
					PIN_PARG(WINDOWS::REGSAM), //   _In_        REGSAM samDesired
					PIN_PARG(WINDOWS::PHKEY), //   _Out_       PHKEY phkResult
					PIN_PARG_END()
					);		
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperRegOpenKeyEx::myRegOpenKeyEx,
					IARG_PROTOTYPE, proto,
					IARG_ORIG_FUNCPTR,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 3,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 4,
					IARG_CONTEXT,
					IARG_BOOL, isUnicode,
					IARG_END
					);
		}

	public:
		WrapperRegOpenKeyEx():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "RegOpenKeyEx");
		}

		~WrapperRegOpenKeyEx(){};
};