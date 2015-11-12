/*
 * CalibLoader.h
 *
 *  Created on: 11.11.2015
 *      Author: andreas
 */

#ifndef FREECV_INCLUDE_FREECV_STEREO_CALIBLOADER_H_
#define FREECV_INCLUDE_FREECV_STEREO_CALIBLOADER_H_
#include <string.h>
#include <vector>

#include "FreeCV/Core/Matrix.h"

namespace fcv {

class CalibLoader {
public:
	CalibLoader();
	virtual ~CalibLoader();

	typedef struct {
		std::vector<float> distCoeff;			// Contains k1, k2, p1, p2, k3
		Matrix3x3f K;							// Intrinsic calibration
		int imgWidth;							// Width and height of camera image, when calibration was done
		int imgHeight;
		Matrixf P1;								// New Projection matrix (contains new camera matrix)
	} CameraCalib;


	static bool loadStereoCalib(std::string fileName, CameraCalib cam1, CameraCalib cam2);
};

} /* namespace fcv */

#endif /* FREECV_INCLUDE_FREECV_STEREO_CALIBLOADER_H_ */
