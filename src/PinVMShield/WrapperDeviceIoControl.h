/*
 * @filename 		WrapperDeviceIoControl.h
 * @version			1.0, January 2014
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap DeviceIoControl Windows API (normally used for detection thru hard disk size)
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

class WrapperDeviceIoControl: public PinWrapperWinAPI
{
	private:
		/**
		 Wrapper of DeviceIoControl Windows API. Spoofs the return if needed
				before returning to the caller 
		 @param hDevice A handle to the device on which the operation is to be performed
		 @param lpInBuffer A pointer to the input buffer that contains the data required to perform the operation. 
		 @param nInBufferSize The size of the input buffer, in bytes
		 @param lpOutBuffer A pointer to the output buffer that is to receive the data returned by the operation. 
		 @param nOutBufferSize The size of the output buffer, in bytes.
		 @param lpBytesReturned A pointer to a variable that receives the size of the data stored in the output buffer, in bytes.
		 @param lpOverlapped A pointer to an OVERLAPPED structure.
		 @return returns non-zero when successful, 0 otherwise
				For more information, check http://msdn.microsoft.com/en-us/library/windows/desktop/aa363216(v=vs.85).aspx
		 */
		static WINDOWS::BOOL myDeviceIoControl(AFUNPTR orig, 
													WINDOWS::HANDLE hDevice, WINDOWS::DWORD dwIoControlCode,
													WINDOWS::LPVOID lpInBuffer, WINDOWS::DWORD nInBufferSize,
													WINDOWS::LPVOID lpOutBuffer, WINDOWS::DWORD nOutBufferSize,
													WINDOWS::LPDWORD lpBytesReturned, WINDOWS::LPOVERLAPPED lpOverlapped,
													CONTEXT *ctx, bool isUnicode)
		{
			WINDOWS::BOOL retVal;
			// TODO Check for detection of some VM, and spoof results if needed
			PIN_CallApplicationFunction(ctx, PIN_ThreadId(),
									CALLINGSTD_STDCALL, orig,
									PIN_PARG(WINDOWS::BOOL), &retVal,
									PIN_PARG(WINDOWS::HANDLE), hDevice,
									PIN_PARG(WINDOWS::DWORD), dwIoControlCode,
									PIN_PARG(WINDOWS::LPVOID), lpInBuffer,
									PIN_PARG(WINDOWS::DWORD), nInBufferSize,
									PIN_PARG(WINDOWS::LPVOID), lpOutBuffer,
									PIN_PARG(WINDOWS::DWORD), nOutBufferSize,
									PIN_PARG(WINDOWS::LPDWORD), lpBytesReturned,
									PIN_PARG(WINDOWS::LPOVERLAPPED), lpOverlapped,
									PIN_PARG_END()
									);
			return retVal;
		}
	protected:
		PROTO GetPrototypeFunction(char *funcName)
		{
			return PROTO_Allocate( 
					PIN_PARG(WINDOWS::BOOL), 
					CALLINGSTD_STDCALL, funcName,
					PIN_PARG(WINDOWS::HANDLE),
					PIN_PARG(WINDOWS::DWORD),
					PIN_PARG(WINDOWS::LPVOID),
					PIN_PARG(WINDOWS::DWORD),
					PIN_PARG(WINDOWS::LPVOID),
					PIN_PARG(WINDOWS::DWORD),
					PIN_PARG(WINDOWS::LPDWORD), 
					PIN_PARG(WINDOWS::LPOVERLAPPED),
					PIN_PARG_END()
					);
		}
		
		void ReplaceFunctionSignature(RTN rtn, PROTO proto, bool isUnicode)
		{
			RTN_ReplaceSignature(rtn, (AFUNPTR) WrapperDeviceIoControl::myDeviceIoControl,
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
		WrapperDeviceIoControl():PinWrapperWinAPI()
		{
			strcpy(this -> funcName, "DeviceIoControl");
		}

		~WrapperDeviceIoControl(){};
};