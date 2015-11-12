/*
 * UndistortAndRectify.h
 *
 *  Created on: 10.11.2015
 *      Author: andreas
 */

#ifndef FREECV_INCLUDE_FREECV_IMAGE_UNDISTORTANDRECTIFY_H_
#define FREECV_INCLUDE_FREECV_IMAGE_UNDISTORTANDRECTIFY_H_

#include "FreeCV/Image/Image.h"
#include "FreeCV/Core/Matrix.h"

namespace fcv {

class UndistortAndRectify {
public:
	UndistortAndRectify();
	virtual ~UndistortAndRectify();

	bool initUndistortRectifyMap(Matrix3x3f K, float k1, float k2, float k3, float p1, float p2,
			Matrix3x3f Knew, int width, int height);

	bool remap(Image* distImg1, Image* distImg2, Image* undistImg1, Image* undistImg2);

private:
		float* mapX;
		float* mapY;
};
}  // namespace fcv

#endif /* FREECV_INCLUDE_FREECV_IMAGE_UNDISTORTANDRECTIFY_H_ */
