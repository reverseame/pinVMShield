/*
 * @filename 		WrapperRegQueryValue.h
 * @version			1.0, December 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap RegQueryValueA/RegQueryValueW Windows APIs
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

class WrapperRegQueryValue: public PinWrapperWinAPI
{
	private:
		/**
		 Wrapper of RegQueryValueA/RegQueryValueW Windows APIs. Spoofs the return if needed
				before returning to the caller 
		 @param origFunc a pointer to the address of the original function (RegQueryValueA/RegQueryValueW)
		 @param hKey a handle to an open registry key
		 @param lpSubKey the name of the subkey of the hKey parameter for which the default value is retrieved 
		 @param lpValue a pointer to a buffer that receives the default value of the specified key
		 @param lpcbValue a pointer to a variable that specifies the size of the buffer pointed to by the lpValue parameter, in bytes
		 @return a long value, the same value that original that returns Windows API RegQueryValueA/RegQueryValueW
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/ms724909(v=vs.85).aspx
		 */
		static WINDOWS::LONG myRegQueryValue(AFUNPTR orig, WINDOWS::HKEY hKey, 
										 WINDOWS::LPCTSTR lpSubKey, WINDOWS::LPTSTR lpValue, 
										 WINDOWS::PLONG lpcbValue, CONTEXT *ctx,
										 bool isUnicode)
		{
			WINDOWS::LONG retVal;
			PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
									CALLINGSTD_STDCALL, orig,
									PIN_PARG(WINDOWS::LONG), &retVal,
									PIN_PARG(WINDOWS::HKEY), hKey,
									PIN_PARG(WINDOWS::LPCTSTR), lpSubKey,
									PIN_PARG(WINDOWS::LPTSTR), lpValue,
									PIN_PARG(WINDOWS::PLONG), lpcbValue,
									PIN_PARG_END()
									);

			bool detectionVMs = checkDetection(ctx, "RegQueryValue", (char *)lpValue, isUnicode);
				//checkDetection((char *)lpValue) || checkDetection((char *)lpSubKey);

			if(detectionVMs)
			{
				printMessage("\t-> [RegQV] Parsing a dubious registry data value\n");
				return -1; // return an error code
			}

			return retVal;
		}
	protected:
		PROTO GetPrototypeFunction(char *funcName)
		{
			return PROTO_Allocate( 
					PIN_PARG(WINDOWS::LONG), 
					CALLINGSTD_STDCALL, funcName,
					PIN_PARG(WINDOWS::HKEY), // _In_         HKEY hKey
					PIN_PARG(WINDOWS::LPCTSTR), //   _In_opt_     LPCTSTR lpSubKey
					PIN_PARG(WINDOWS::LPTSTR), //   _Out_opt_    LPTSTR lpValue
					PIN_PARG(WINDOWS::PLONG), // _Inout_opt_  PLONG lpcbValue
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperRegQueryValue::myRegQueryValue,
					IARG_PROTOTYPE, proto,
					IARG_ORIG_FUNCPTR,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 3,
					IARG_CONTEXT,
					IARG_BOOL, isUnicode,
					IARG_END
					);
		}

	public:
		WrapperRegQueryValue():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "RegQueryValue");
		}

		~WrapperRegQueryValue(){};
};