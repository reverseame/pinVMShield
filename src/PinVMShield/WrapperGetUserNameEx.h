/*
 * @filename 		WrapperGetUserNameEx.h
 * @version			1.0, December 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap GetUserNameExA/GetUserNameExW Windows APIs
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

class WrapperGetUserNameEx: public PinWrapperWinAPI
{
	private:
		/**
		 Wrapper of GetUserNameExA/GetUserNameExW Windows APIs. Spoofs the return if needed
				before returning to the caller
		 @param origFunc a pointer to the address of the original function (GetModuleHandleA/GetModuleHandleW)
		 @param NameFormat the format of the name. This parameter is a value from the EXTENDED_NAME_FORMAT enumeration type
						XXX Change it to INT type by a Pin error. Not tested!
		 @param lpBuffer a pointer to the buffer to receive the user's logon name
		 @param lpnSize on input, this variable specifies the size of the lpBuffer buffer, in TCHARs. 
						On output, the variable receives the number of TCHARs copied to the buffer, including the terminating null character
		 @param ctx a pointer to a CONTEXT Pin structure that stores the architectural state of the processor
		 @return a bool indicating whether the execution was successful
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/ms724435(v=vs.85).aspx
		 */
		static WINDOWS::BOOL myGetUserNameEx(AFUNPTR orig, 
									WINDOWS::INT NameFormat, WINDOWS::LPTSTR lpBuffer, 
									WINDOWS::LPDWORD lpnSize, CONTEXT *ctx,
									bool isUnicode)
		{
			WINDOWS::BOOL retVal;
			PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
									CALLINGSTD_STDCALL, orig,
									PIN_PARG(WINDOWS::BOOL), &retVal,
									PIN_PARG(WINDOWS::INT), NameFormat,
									PIN_PARG(WINDOWS::LPTSTR), lpBuffer,
									PIN_PARG(WINDOWS::LPDWORD), lpnSize,
									PIN_PARG_END()
									);
			
			bool detectionVMs = checkDetection(ctx, "GetUserNameEx", (char *)lpBuffer, isUnicode);

			if(detectionVMs)
			{
				printMessage("\t-> [GUNE] Asked for a dubious username\n");
				return 0; // error code
			}

			return retVal;
		}
	protected:
		PROTO GetPrototypeFunction(char *funcName)
		{
			return PROTO_Allocate( 
					PIN_PARG(WINDOWS::BOOLEAN), 
					CALLINGSTD_STDCALL, funcName,
					PIN_PARG(WINDOWS::INT),
					PIN_PARG(WINDOWS::LPTSTR),
					PIN_PARG(WINDOWS::LPDWORD),
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperGetUserNameEx::myGetUserNameEx,
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
		WrapperGetUserNameEx():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "GetUserNameEx");
		}

		~WrapperGetUserNameEx(){};
};