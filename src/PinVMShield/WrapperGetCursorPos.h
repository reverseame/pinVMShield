/*
 * @filename 		WrapperGetCursorPos.h
 * @version			1.0, December 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap GetCursorPos Windows API
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

class WrapperGetCursorPos: public PinWrapperWinAPI
{
	private:
		/**
		 Wrapper of GetCursorPos Windows APIs. Spoofs the return if needed
				before returning to the caller 
		 @param lpPoint a pointer to a POINT structure that receives the screen coordinates of the cursor
		 @return returns zero, the same value that original when Windows API GetCursorPos fails
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/ms648390(v=vs.85).aspx
		 */
		static WINDOWS::BOOL myGetCursorPos(AFUNPTR orig, 
										WINDOWS::LPPOINT lpPoint, 
										CONTEXT *ctx,
										bool isUnicode)
		{
			/*WINDOWS::BOOL retVal;
			PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
									CALLINGSTD_STDCALL, orig,
									PIN_PARG(WINDOWS::BOOL), &retVal,
									PIN_PARG(WINDOWS::LPPOINT), lpPoint,
									PIN_PARG_END()
									);
			*/

			return false;
		}
	protected:
		PROTO GetPrototypeFunction(char *funcName)
		{
			return PROTO_Allocate( 
					PIN_PARG(WINDOWS::BOOL), 
					CALLINGSTD_STDCALL, funcName,
					PIN_PARG(WINDOWS::LPPOINT), // _OUT_      LPPOINT lpPoint
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperGetCursorPos::myGetCursorPos,
					IARG_PROTOTYPE, proto,
					IARG_ORIG_FUNCPTR,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_CONTEXT,
					IARG_BOOL, isUnicode,
					IARG_END
					);
		}
		
		RTN FindRTNByNameA(IMG img){
			RTN rtn = RTN_FindByName(img, this -> funcName);
			return rtn;
		}

		RTN FindRTNByNameW(IMG img){
			RTN rtn = RTN_FindByName(img, this -> funcName); // Repeat...
			return rtn;
		}

		virtual PROTO GetPrototypeFunctionA()
		{
			PROTO proto = GetPrototypeFunction(this -> funcName);
			return proto;
		}

	public:
		WrapperGetCursorPos():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "GetCursorPos");
		}

		~WrapperGetCursorPos(){};
};
  
					