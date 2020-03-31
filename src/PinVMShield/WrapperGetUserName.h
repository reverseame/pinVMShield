/*
 * @filename 		WrapperGetUserName.h
 * @version			1.0, December 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap GetUserNameA/GetUserNameW Windows APIs
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

class WrapperGetUserName: public PinWrapperWinAPI
{
	private:
		/**
		 Wrapper of GetUserNameA/GetUserNameW Windows APIs. Spoofs the return if needed
				before returning to the caller
		 @param origFunc a pointer to the address of the original function (GetModuleHandleA/GetModuleHandleW)
		 @param lpBuffer a pointer to the buffer to receive the user's logon name
		 @param lpnSize on input, this variable specifies the size of the lpBuffer buffer, in TCHARs. 
						On output, the variable receives the number of TCHARs copied to the buffer, including the terminating null character
		 @param ctx a pointer to a CONTEXT Pin structure that stores the architectural state of the processor
		 @return a bool indicating whether the execution was successful
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/ms724432(v=vs.85).aspx
		 */
		static WINDOWS::BOOL myGetUserName(AFUNPTR orig, 
										   WINDOWS::LPTSTR lpBuffer, WINDOWS::LPDWORD lpnSize,
										   CONTEXT *ctx,
										   bool isUnicode)
		{
			WINDOWS::BOOL retVal;
			PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
									CALLINGSTD_STDCALL, orig,
									PIN_PARG(WINDOWS::BOOL), &retVal,
									PIN_PARG(WINDOWS::LPTSTR), lpBuffer,
									PIN_PARG(WINDOWS::LPDWORD), lpnSize,
									PIN_PARG_END()
									);

			bool detectionVMs = checkDetection(ctx, "GetUserName", (char *)lpBuffer, isUnicode);

			if(detectionVMs)
			{
				printMessage("\t-> [GUN] Asked for a dubious username, spoofing return value\n");
				strcpy((char *)lpBuffer, "LadyBoysLover"); // XXX: Improve this, put some randomness...
			}

			return retVal;
		}
	protected:
		PROTO GetPrototypeFunction(char *funcName)
		{
			return PROTO_Allocate( 
					PIN_PARG(WINDOWS::BOOL), 
					CALLINGSTD_STDCALL, funcName,
					PIN_PARG(WINDOWS::LPTSTR),
					PIN_PARG(WINDOWS::LPDWORD),
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperGetUserName::myGetUserName,
					IARG_PROTOTYPE, proto,
					IARG_ORIG_FUNCPTR,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
					IARG_CONTEXT,
					IARG_BOOL, isUnicode,
					IARG_END
					);
		}

	public:
		WrapperGetUserName():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "GetUserName");
		}

		~WrapperGetUserName(){};
};