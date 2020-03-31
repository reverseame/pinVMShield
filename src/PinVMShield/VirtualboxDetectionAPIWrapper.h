/*
 * @filename 		VirtualboxDetectionAPIWrapper.h
 * @version			1.0, December 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Concrete class to avoid malware detection of VirtualBox
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
#include "DetectionAPIWrapper.h"

class VirtualboxDetectionAPIWrapper: public DetectionAPIWrapper
{
	public:
		VirtualboxDetectionAPIWrapper():DetectionAPIWrapper()
		{
		}

		~VirtualboxDetectionAPIWrapper(){};

		/**
		 Function used to check a string comparing it with different values related to detection of Virtualbox
		 @param s the string to be checked
		 @return a boolean indicating whether s is presumably a string related to some kind of detection
		*/
		bool triesDetection(char *s)
		{
			return	(stristr(s, "vbox") != NULL) || 
					(stristr(s, "virtualbox") != NULL);
		}
};