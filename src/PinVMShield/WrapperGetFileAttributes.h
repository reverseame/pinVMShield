/*
 * @filename 		WrapperGetFileAttributes.h
 * @version			1.0, December 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap WrapperGetFileAttributes Windows API
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

class WrapperGetFileAttributes: public PinWrapperWinAPI
{
	private:
		/**
		 Wrapper of GetFileAttributes Windows APIs. Spoofs the return if needed
				before returning to the caller
		 @param origFunc a pointer to the address of the original function (GetFileAttributes)
		 @param lpFileName the name of the file or directory
		 @param ctx a pointer to a CONTEXT Pin structure that stores the architectural state of the processor
		 @return a long value, the same value that original that returns Windows API GetFileAttributes
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/aa364944(v=vs.85).aspx
		 */
		static WINDOWS::LONG myGetFileAttributes(AFUNPTR orig, 
										 WINDOWS::LPCTSTR lpFileName, 
										 CONTEXT *ctx,
										 bool isUnicode)
		{
			WINDOWS::LONG retVal;
			PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
									CALLINGSTD_STDCALL, orig,
									PIN_PARG(WINDOWS::LONG), &retVal,
									PIN_PARG(WINDOWS::LPCTSTR), lpFileName,
									PIN_PARG_END()
									);

			bool detectionVMs = checkDetection(ctx, "GetFileAttributes", (char *)lpFileName, isUnicode);

			if(detectionVMs)
			{
				printMessage("\t-> [GFA] Parsing a dubious filename\n");
				return -1;
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
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperGetFileAttributes::myGetFileAttributes,
					IARG_PROTOTYPE, proto,
					IARG_ORIG_FUNCPTR,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_CONTEXT,
					IARG_BOOL, isUnicode,
					IARG_END
					);
		}

	public:
		WrapperGetFileAttributes():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "GetFileAttributes");
		}

		~WrapperGetFileAttributes(){};
};