/*
 * @filename 		DetectionAPIWrapper.h
 * @version			1.0, December 2013
 * @author			R.J. Rodríguez (rjrodriguez@fi.upm.es), I. Rodríguez-Gastón (irodriguez@virtualminds.es)
 * @description		Abstract class for creating new wrappers for avoid malware detection
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
#pragma once
namespace WINDOWS{
	#include <windows.h>
	#include <shlwapi.h>
}
#include <string.h>

#include "DetectionWrapper.h"

class DetectionAPIWrapper: public DetectionWrapper
{
public:
	DetectionAPIWrapper();
	~DetectionAPIWrapper();

	// Abstract method, to be implemented by childs
	virtual bool triesDetection(char *s) = 0;
};

DetectionAPIWrapper::DetectionAPIWrapper(){}
