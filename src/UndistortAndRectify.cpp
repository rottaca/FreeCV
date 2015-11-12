/*
 * UndistortAndRectify.cpp
 *
 *  Created on: 10.11.2015
 *      Author: andreas
 */

#include "FreeCV/Stereo/UndistortAndRectify.h"

namespace fcv {
UndistortAndRectify::UndistortAndRectify() {
	mapX = NULL;
	mapY = NULL;

}

UndistortAndRectify::~UndistortAndRectify() {
}



bool UndistortAndRectify::initUndistortRectifyMap(Matrix3x3f K, float k1, float k2, float k3, float p1, float p2,
		Matrix3x3f Knew, int width, int height)
{
	// distorted camera matrix
	float cX = K.at(0,2),
		  cY = K.at(1,2),
		  fX = K.at(0,0),
		  fY = K.at(1,1);
	// Optimal camera matrix
	float cX_ = Knew.at(0,2),
		  cY_ = Knew.at(1,2),
		  fX_ = Knew.at(0,0),
		  fY_ = Knew.at(1,1);

	if(mapX == NULL)
		delete[] mapX;
	if(mapY == NULL)
		delete[] mapY;

	mapX = new float[width*height];
	mapY = new float[width*height];

	float* mapXPtr = mapX;
	float* mapYPtr = mapY;

	float xU,yU, 		// Undistorted coordinates
		  xD,yD,		// Distorted coordinates
		  rU2,xU2,yU2,xyU2;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			xU = (x - cX_)/fX_;
			yU = (y - cY_)/fY_;

			xU2 = xU*xU;
			yU2 = yU*yU;
			xyU2 = 2*xU*yU;
			rU2 = xU2 + yU2;
			// Apply distortion function
			xD = xU*(1 + (k1 + (k2 + k3*rU2)*rU2)*rU2) + p1*xyU2 + p2*(rU2 + 2*xU2);
			yD = yU*(1 + (k1 + (k2 + k3*rU2)*rU2)*rU2) + p2*xyU2 + p1*(rU2 + 2*yU2);

			// Reprojected x
			*mapXPtr++ = xD*fX + cX;
			// Reprojected y
			*mapYPtr++ = yD*fY + cY;
		}
	}

	return true;
}
}
