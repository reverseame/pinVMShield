/*
 * @filename 		EIPDetectionInsWrapper.h
 * @version			1.1, May 2014
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to avoid malware detection of DBI frameworks checking real EIP values when executing
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

class EIPDetectionInsWrapper: public DetectionInsWrapper
{
	public:
		EIPDetectionInsWrapper():DetectionInsWrapper()
		{
		}

		~EIPDetectionInsWrapper(){};

		/**
		 Function used to check a string comparing it with different values related to detection of debuggers
		 @param s the string to be checked
		 @return a boolean indicating whether s is presumably a string related to some kind of detection
		*/
		bool triesDetection(OPCODE opcode, CONTEXT *ctx)
		{
			bool detects = false;
			// Check if opcode is FSTENV, FNSTENV, FSAVE, FXSAVE
			// If true, then spoof return
			// if (opcode == FSTENV || opcode == FNSTENV){ // FSTENV or FNSTENV
				detects = true;
			//}else if (opcode == FSAVE){ // FSAVE
				detects = true;
			//}else if (opcode == FXSAVE){ // FXSAVE
				detects = true;
			return	detects;
		}
};