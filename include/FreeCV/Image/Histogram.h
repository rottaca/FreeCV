/*
 * Histogram.h
 *
 *  Created on: 12.10.2015
 *      Author: andreas
 */

#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "FreeCV/Image/Image.h"
#include "FreeCV/Core/Vector.h"
#include "FreeCV/Core/Rectangle.h"
namespace fcv {

Vectori calcHist(Image* inputImage);
Vectori calcHist(Image* inputImage, Rectangle roi);

Vectori calcHistCum(Vectori hist);

bool equalizeHistogram(Image* inputImage, Image* outputImage);
bool equalizeHistogram(Image* inputImage, Image* outputImage, Rectangle roi);

Image convertHistToImage(Vectori* hist, int height = 100);

} /* namespace fcv */

#endif /* HISTOGRAM_H_ */
