/*
 * @filename 		WrapperDescriptorTable.h
 * @version			1.1, May 2014
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to wrap descriptor table instructions (IDT, LDT, GDT)
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
#include "DescriptorTableDetectionInsWrapper.h"


class WrapperDescriptorTable: public PinWrapperInstruction
{
private:
	static bool foolResult(OPCODE opcode, CONTEXT  *ctx)
	{
		DescriptorTableDetectionInsWrapper* detWrapper = new DescriptorTableDetectionInsWrapper();
		bool detects = detWrapper -> triesDetection(opcode, ctx);
		if(detects)
			if (opcode & 0x0F0100){ // SIDT or SGDT
				// eax = 0xC0000000
				PIN_SetContextReg(ctx, REG_EAX, 0x0C000000);
			}else if (opcode & 0x0F0000){ // SLDT
				// eax = 0x00000000
				PIN_SetContextReg(ctx, REG_EAX, 0x00000000);
			}
		return true;
	}

protected:
	void makeInstrumentation(INS ins)
	{
		
		//if(INS_Opcode(ins) == SIDT || INS_Opcode(ins) == SGDT || INS_Opcode(ins) == SLDT)
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)WrapperDescriptorTable::foolResult, 
			IARG_UINT32, INS_Opcode(ins), IARG_CONTEXT, IARG_END);
	}

public:

	WrapperDescriptorTable():PinWrapperInstruction()
	{
	}
	~WrapperDescriptorTable();
};