/*
 * @filename 		WrapperFPUEnvironment.h
 * @version			1.1, May 2014
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap FPU environment
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

#pragma once
#include "pin.H"

#include "common.h"

// Detection wrappers
#include "EIPDetectionInsWrapper.h"


class WrapperFPUEnvironment: public PinWrapperInstruction
{
private:
	static bool foolResult(OPCODE opcode, CONTEXT  *ctx)
	{
		EIPDetectionInsWrapper* detWrapper = new EIPDetectionInsWrapper();
		bool detects = detWrapper -> triesDetection(opcode, ctx);
		if(detects)
		{
			ADDRINT value = 0x00401000; // XXX you should substitute this by the value it's checking in...
			// I guess also you should add more logic here, in case more checkings are performed
			PIN_SetContextReg(ctx, REG_EIP, value);
		}
		return true;
	}

protected:
	void makeInstrumentation(INS ins)
	{
		
		//if (...) TODO
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)WrapperFPUEnvironment::foolResult, 
			IARG_UINT32, INS_Opcode(ins), IARG_CONTEXT, IARG_END);
	}

public:

	WrapperFPUEnvironment():PinWrapperInstruction()
	{
	}
	~WrapperFPUEnvironment();
};