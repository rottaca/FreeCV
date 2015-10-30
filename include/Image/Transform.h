/*
 * Transform.h
 *
 *  Created on: 13.10.2015
 *      Author: andreas
 */

#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "Image/Image.h"
#include "Core/Vector.h"

namespace fcv {

bool imagePositionAltitudeTransform(Image* inputImage, Image* outputImage, float angle, Vector2f t, Vector2f s);

bool imageHoughTransform(Image* inputBinaryImage,Image* outputHoughImage);

} /* namespace fcv */

#endif /* TRANSFORM_H_ */
