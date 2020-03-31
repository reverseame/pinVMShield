/*
 * @filename 		WrapperCallNamedPipe.h
 * @version			1.1, March 2014
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap CallNamedPipe Windows APIs
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

class WrapperCallNamedPipe: public PinWrapperWinAPI
{
	private:
		/**
		 Wrapper of CallNamedPipe Windows APIs. Spoofs the return if needed
				before returning to the caller
		 @param origFunc a pointer to the address of the original function (CallNamedPipe)
		 @param lpNamedPipeName The pipe name.
		 @param lpInBuffer The data to be written to the piper
		 @param nInBufferSize The size of the write buffer, in bytes.
		 @param lpOutBuffer A pointer to the buffer that receives the data read from the pipe.
		 @param nOutBufferSize The size of the read buffer, in bytes.
		 @param lpBytesRead A pointer to a variable that receives the number of bytes read from the pipe.
		 @param nTimeOut The number of milliseconds to wait for the named pipe to be available.
		 @return If the function succeeds, the return value is nonzero. 
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/aa365144(v=vs.85).aspx
		 */
		static WINDOWS::BOOL myCallNamedPipe(AFUNPTR orig, 
			WINDOWS::LPCTSTR lpNamedPipeName, WINDOWS::LPVOID lpInBuffer, WINDOWS::DWORD nInBufferSize,
			WINDOWS::LPVOID lpOutBuffer, WINDOWS::DWORD nOutBufferSize, WINDOWS::LPDWORD lpBytesRead, 
			WINDOWS::DWORD nTimeOut, CONTEXT *ctx, bool isUnicode)
		{
			WINDOWS::BOOL retVal;
			PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
									CALLINGSTD_STDCALL, orig,
									PIN_PARG(WINDOWS::BOOL), &retVal,
									PIN_PARG(WINDOWS::LPCTSTR), lpNamedPipeName,
									PIN_PARG(WINDOWS::LPVOID), lpInBuffer,
									PIN_PARG(WINDOWS::DWORD), nInBufferSize,
									PIN_PARG(WINDOWS::LPVOID), lpOutBuffer,
									PIN_PARG(WINDOWS::DWORD), nOutBufferSize,
									PIN_PARG(WINDOWS::LPDWORD), lpBytesRead,
									PIN_PARG(WINDOWS::DWORD), nTimeOut,
									PIN_PARG_END()
									);
			bool detectionVMs = checkDetection(ctx, "CallNamedPipe", (char *)lpNamedPipeName, isUnicode);

			if(detectionVMs)
			{
				printMessage("\t-> [CNP] Asked for a dubious module\n");
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
					PIN_PARG(WINDOWS::LPCTSTR),
					PIN_PARG(WINDOWS::LPVOID), 
					PIN_PARG(WINDOWS::DWORD), 
					PIN_PARG(WINDOWS::LPVOID), 
					PIN_PARG(WINDOWS::DWORD), 
					PIN_PARG(WINDOWS::LPDWORD), 
					PIN_PARG(WINDOWS::DWORD), 
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperCallNamedPipe::myCallNamedPipe,
					IARG_PROTOTYPE, proto,
					IARG_ORIG_FUNCPTR,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 3,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 4,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 5,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 6,
					IARG_CONTEXT,
					IARG_BOOL, isUnicode,
					IARG_END
					);
		}

	public:
		WrapperCallNamedPipe():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "CallNamedPipe");
		}

		~WrapperCallNamedPipe(){};
};