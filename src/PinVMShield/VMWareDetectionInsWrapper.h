/*
 * @filename 		VMWareDetectionInsWrapper.h
 * @version			1.1, May 2014
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to avoid malware detection of VMWare using assembler instructions
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
#include "DetectionInsWrapper.h"

class VMWareDetectionInsWrapper: public DetectionInsWrapper
{
	public:
		VMWareDetectionInsWrapper():DetectionInsWrapper()
		{
		}

		~VMWareDetectionInsWrapper(){};

		/**
		 Function used to check a string comparing it with different values related to detection of debuggers
		 @param s the string to be checked
		 @return a boolean indicating whether s is presumably a string related to some kind of detection
		*/
		bool triesDetection(OPCODE opcode, CONTEXT *ctx)
		{
			bool detects = false;
			// Check if opcode is IN
			// If true, then spoof return
			// if (opcode == IN && edx == 0x5658 && eax == 0x564D5868){ // IN
				detects = true;
				// ebx = 0xDEADBEEF;
			//}
			return	detects;
		}
};