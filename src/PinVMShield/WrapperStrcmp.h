/*
 * @filename 		WrapperStrcmp.h
 * @version			1.1, April 2014
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap lstrcmpA/lstrcmpW Windows APIs
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

class WrapperStrcmp: public PinWrapperWinAPI
{
	private:
		/**
		 Wrapper of CompareStringA/CompareStringW Windows APIs. Spoofs the return if needed
				before returning to the caller 
		 @param lpString1 The first null-terminated string to be compared.
		 @param lpString2 The second null-terminated string to be compared.
		 @return If the string pointed to by lpString1 is less than the string pointed to by lpString2, the return value is negative. 
		         If the string pointed to by lpString1 is greater than the string pointed to by lpString2, the return value is positive. 
				 If the strings are equal, the return value is zero.
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/ms647488(v=vs.85).aspx
		 */
		static int myStrcmp(AFUNPTR orig, 
								WINDOWS::LPCTSTR lpString1, WINDOWS::LPCTSTR lpString2, 
								CONTEXT *ctx, bool isUnicode)
		{
			int retVal;
			bool detectionVMs = checkDetection(ctx, "lstrcmp", (char *)lpString1, isUnicode) ||
								checkDetection(ctx, "lstrcmp", (char *)lpString2, isUnicode);

			if(detectionVMs)
			{
				printMessage("\t-> [lstrcmp] Asked for a dubious string, spoofing return\n");
				return -1;
			}else
				PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
									CALLINGSTD_STDCALL, orig,
									PIN_PARG(int), &retVal,
									PIN_PARG(WINDOWS::LPCTSTR), lpString1,
									PIN_PARG(WINDOWS::LPCTSTR), lpString2,
									PIN_PARG_END()
									);

			return retVal;
		}
	protected:
		PROTO GetPrototypeFunction(char *funcName)
		{
			return PROTO_Allocate( 
					PIN_PARG(int), 
					CALLINGSTD_STDCALL, funcName,
					PIN_PARG(WINDOWS::LPCTSTR),
					PIN_PARG(WINDOWS::LPCTSTR),
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperStrcmp::myStrcmp,
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
		WrapperStrcmp():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "lstrcmp");
		}

		~WrapperStrcmp(){};
};