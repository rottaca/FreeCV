/*
 * Filter.h
 *
 *  Created on: 11.10.2015
 *      Author: andreas
 */

#ifndef FILTER_H_
#define FILTER_H_

#include "Core/Matrix.h"
#include "Image/Image.h"

#include <stdlib.h>
#include <assert.h>
#include <limits>
namespace fcv {

//typedef enum {
//	BT_ZERO, 			// All pixels outside the image are set to zero
//	BT_CLAMP,			// repeat edge pixels indefinitely
//	BT_WARP,			// loop around the image in a "toroidal" configuration
//	BT_MIRROR,			// reflect pixels across the image edge
//} BorderTreatment;

template <typename T>
inline T clip(const T& n, const T& lower, const T& upper) {
  return std::max(lower, std::min(n, upper));
}

bool filterMedian(Image* inputImage, Image* outputImage, int kernelSize);
bool filterGauss(Image* inputImage, Image* outputImage, int kernelSize);

bool applyKernel(Image* inputImage, Image* outputImage, Matrixi& kernel, int weightSum);
} /* namespace fcv */

#endif /* FILTER_H_ */
