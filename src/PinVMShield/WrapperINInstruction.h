/*
 * @filename 		WrapperINInstruction.h
 * @version			1.1, May 2014
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap IN instructions (used to detect VMWare comm. channel)
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
#include "VMWareDetectionInsWrapper.h"


class WrapperINInstruction: public PinWrapperInstruction
{
private:
	static bool foolResult(OPCODE opcode, CONTEXT  *ctx)
	{
		VMWareDetectionInsWrapper* detWrapper = new VMWareDetectionInsWrapper();
		bool detects = detWrapper -> triesDetection(opcode, ctx);
		if(detects)
		{
			//PIN_SetContextReg(ctx, REG_XXX, YYY);
		}
		return true;
	}
protected:
	void makeInstrumentation(INS ins)
	{
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)WrapperINInstruction::foolResult, 
			IARG_UINT32, INS_Opcode(ins), IARG_CONTEXT, IARG_END);
	}
public:

	WrapperINInstruction();
	~WrapperINInstruction();
};

WrapperINInstruction::WrapperINInstruction()
{
}