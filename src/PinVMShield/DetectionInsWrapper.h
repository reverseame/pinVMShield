/*
 * @filename 		DetectionInsWrapper.h
 * @version			1.1, April 2014
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
namespace WINDOWS{
	#include <windows.h>
	#include <shlwapi.h>
}
#include <string.h>

#include "DetectionWrapper.h"

class DetectionInsWrapper: public DetectionWrapper
{
public:
	DetectionInsWrapper();
	~DetectionInsWrapper();

	// Abstract method, to be implemented by childs
	virtual bool triesDetection(OPCODE opcode, CONTEXT *ctx) = 0;
};

DetectionInsWrapper::DetectionInsWrapper(){}
