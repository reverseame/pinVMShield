/*
 * @filename 		WrapperCreateNamedPipe.h
 * @version			1.0, December 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap CreateNamedPipeA/CreateNamedPipeW Windows APIs
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

class WrapperCreateNamedPipe: public PinWrapperWinAPI
{
	private:
		/**
		 Wrapper of CreateNamedPipeA/CreateNamedPipeW Windows APIs. Spoofs the return if needed
				before returning to the caller 
		 @param lpName a unique pipe name
		 @param dwOpenMode the open mode
		 @param dwPipeMode the pipe mode
		 @param nMaxInstances the maximum number of instances that can be created for this pipe
		 @param nOutBufferSize the number of bytes to reserve for the output buffer
		 @param nInBufferSize the number of bytes to reserve for the input buffer
		 @param nDefaultTimeOut the default time-out value, in milliseconds, if the 
		        WaitNamedPipe function specifies NMPWAIT_USE_DEFAULT_WAIT
		 @param lpSecurityAttributes A pointer to a SECURITY_ATTRIBUTES structure that specifies a security descriptor for the new named pipe 
		        and determines whether child processes can inherit the returned handle
		 @return an open handle to the specified pipe, the same value that original that returns Windows API CreateNamedPipeA/CreateNamedPipeW
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/aa365150(v=vs.85).aspx
		 */
		static WINDOWS::HANDLE myCreateNamedPipe(AFUNPTR orig, 
										WINDOWS::LPCTSTR lpName, WINDOWS::DWORD dwOpenMode,
										WINDOWS::DWORD dwPipeMode, WINDOWS::DWORD nMaxInstances, 
										WINDOWS::DWORD nOutBufferSize, WINDOWS::DWORD nInBufferSize, 
										WINDOWS::DWORD nDefaultTimeOut, WINDOWS::LPSECURITY_ATTRIBUTES lpSecurityAttributes,
										CONTEXT *ctx,
										bool isUnicode)
		{
			WINDOWS::HANDLE retVal;
			PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
									CALLINGSTD_STDCALL, orig,
									PIN_PARG(WINDOWS::HANDLE), &retVal,
									PIN_PARG(WINDOWS::LPCTSTR), lpName,
									PIN_PARG(WINDOWS::DWORD), dwOpenMode,
									PIN_PARG(WINDOWS::DWORD), dwPipeMode,
									PIN_PARG(WINDOWS::DWORD), nMaxInstances,
									PIN_PARG(WINDOWS::DWORD), nOutBufferSize,
									PIN_PARG(WINDOWS::DWORD), nInBufferSize,
									PIN_PARG(WINDOWS::DWORD), nDefaultTimeOut,
									PIN_PARG(WINDOWS::LPSECURITY_ATTRIBUTES), lpSecurityAttributes,
									PIN_PARG_END()
									);

			bool detectionVMs = checkDetection(ctx, "CreateNamedPipe", (char *)lpName, isUnicode);

			if(detectionVMs)
			{
				printMessage("\t-> [CNP] Asked for a dubious pipe name, spoofing return\n");
				return (WINDOWS::HANDLE) -1;
			}

			return retVal;
		}
	protected:
		PROTO GetPrototypeFunction(char *funcName)
		{
			return PROTO_Allocate( 
					PIN_PARG(WINDOWS::LONG), 
					CALLINGSTD_STDCALL, funcName,
					PIN_PARG(WINDOWS::LPCTSTR), // _In_      LPCTSTR lpName
					PIN_PARG(WINDOWS::DWORD), // _In_      DWORD dwOpenMode
					PIN_PARG(WINDOWS::DWORD), // _In_      DWORD dwPipeMode
					PIN_PARG(WINDOWS::DWORD), // _In_      DWORD nMaxInstances
					PIN_PARG(WINDOWS::DWORD), // _In_      DWORD nOutBufferSize
					PIN_PARG(WINDOWS::DWORD), // _In_      DWORD nInBufferSize
					PIN_PARG(WINDOWS::DWORD), // _In_      DWORD nDefaultTimeOut
					PIN_PARG(WINDOWS::LPSECURITY_ATTRIBUTES), // _In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperCreateNamedPipe::myCreateNamedPipe,
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
					IARG_CONTEXT,
					IARG_BOOL, isUnicode,
					IARG_END
					);
		}

	public:
		WrapperCreateNamedPipe():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "CreateNamedPipe");
		}

		~WrapperCreateNamedPipe(){};
};
  
					