/*
 * @filename 		WrapperCompareStringEx.h
 * @version			1.1, April 2014
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap CompareStringExA/CompareStringExW Windows APIs
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

class WrapperCompareStringEx: public PinWrapperWinAPI
{
	private:
		/**
		 Wrapper of CompareStringExA/CompareStringExW Windows APIs. Spoofs the return if needed
				before returning to the caller 
		 @param Locale Locale identifier of the locale used for the comparison.
		 @param dwCmpFlags Flags that indicate how the function compares the two strings
		 @param lpString1 Pointer to the first string to compare
		 @param cchCount1 Length of the string indicated by lpString1, excluding the terminating null character
		 @param lpString2 Pointer to the second string to compare
		 @param cchCount2 Length of the string indicated by lpString2, excluding the terminating null character
		 @param lpVersionInformation Reserved; must set to NULL
		 @param lpReserved Reserved; must set to NULL
		 @param lParam Reserved; must be set to 0.
		 @return of <0 The string indicated by lpString1 is less in lexical value than the string indicated by lpString2; 
					=0 The string indicated by lpString1 is equivalent in lexical value to the string indicated by lpString2. 
					The two strings are equivalent for sorting purposes, although not necessarily identical.
					>0 The string indicated by lpString1 is greater in lexical value than the string indicated by lpString2.
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/dd317759(v=vs.85).aspx
		 */
		static int myCompareStringEx(AFUNPTR orig, 
										WINDOWS::LCID Locale, WINDOWS::DWORD dwCmpFlags,
										WINDOWS::LPCTSTR lpString1, int cchCount1,
										WINDOWS::LPCTSTR lpString2, int cchCount2,
										WINDOWS::LPNLSVERSIONINFO lpVersionInformation, WINDOWS::LPVOID lpReserved,
										WINDOWS::LPARAM lParam,
										CONTEXT *ctx, bool isUnicode)
		{
			int retVal;
			bool detectionVMs = checkDetection(ctx, "CompareStringEx", (char *)lpString1, isUnicode) ||
								checkDetection(ctx, "CompareStringEx", (char *)lpString2, isUnicode);

			if(detectionVMs)
			{
				printMessage("\t-> [CSE] Asked for a dubious string, spoofing return\n");
				return -1;
			}else
				PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
									CALLINGSTD_STDCALL, orig,
									PIN_PARG(int), &retVal,
									PIN_PARG(WINDOWS::LCID), Locale,
									PIN_PARG(WINDOWS::DWORD), dwCmpFlags,
									PIN_PARG(WINDOWS::LPCTSTR), lpString1,
									PIN_PARG(int), cchCount1,
									PIN_PARG(WINDOWS::LPCTSTR), lpString2,
									PIN_PARG(int), cchCount2,
									PIN_PARG(WINDOWS::LPNLSVERSIONINFO), lpVersionInformation,
									PIN_PARG(WINDOWS::LPVOID), lpReserved,
									PIN_PARG(WINDOWS::LPARAM), lParam,
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
					PIN_PARG(WINDOWS::LCID),
					PIN_PARG(WINDOWS::DWORD),
					PIN_PARG(WINDOWS::LPCTSTR),
					PIN_PARG(int),
					PIN_PARG(WINDOWS::LPCTSTR),
					PIN_PARG(int),
					PIN_PARG(WINDOWS::LPNLSVERSIONINFO),
					PIN_PARG(WINDOWS::LPVOID), 
					PIN_PARG(WINDOWS::LPARAM),
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperCompareStringEx::myCompareStringEx,
					IARG_PROTOTYPE, proto,
					IARG_ORIG_FUNCPTR,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 3,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 4,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 5,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 6,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 7,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 8,
					IARG_CONTEXT,
					IARG_BOOL, isUnicode,
					IARG_END
					);
		}

	public:
		WrapperCompareStringEx():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "CompareStringEx");
		}

		~WrapperCompareStringEx(){};
};