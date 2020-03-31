/*
 * @filename 		PinWrapperInstruction.h
 * @version			1.1, April 2014
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Abstract class for creating new wrappers for Windows APIs to be spoofed by Pin
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
#include <iostream>
#include <wchar.h>

#include "common.h"

// Detection wrappers
#include "DescriptorTableDetectionInsWrapper.h"

namespace WINDOWS{
	#include <windows.h>
}

class PinWrapperInstruction: public PinWrapperWindows
{
private:

protected:
	//static vector<DetectionInsWrapper*> createDetectionAvoidanceVector();
	
public:

	PinWrapperInstruction();
	~PinWrapperInstruction();
	//void InstrumentInsLevel(INS ins);
	//virtual bool foolResult(CONTEXT *ctx) = 0;
	virtual void makeInstrumentation(INS ins) = 0;
};

PinWrapperInstruction::PinWrapperInstruction()
{
}

/**
   Function used to create a vector with all detections that need to be checked somehow. See specific classes for more details.
   @return a vector with all detections that need to be checked
 */
/*vector<DetectionInsWrapper*> PinWrapperInstruction::createDetectionAvoidanceVector()
{
	vector<DetectionInsWrapper*>  v;
	//v.push_back(new VirtualboxDetectionWrapper());

	v.push_back(new DescriptorTableDetectionInsWrapper());

	return v;
}*/

/*void PinWrapperInstruction::InstrumentInsLevel(INS ins)
{
	bool keepLooking = true;
	vector<DetectionInsWrapper*> vInsDet = createDetectionAvoidanceVector();

	for(vector<DetectionInsWrapper*>::iterator it = vInsDet.begin(); it != vInsDet.end() && keepLooking; it++)
	{
		//keepLooking = !((*it) == INS_Opcode(ins));
	}

	// If matches, instrument...
	if(!keepLooking)
		makeInstrumentation(ins);
}*/

