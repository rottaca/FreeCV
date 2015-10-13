/*
 * Histogram.h
 *
 *  Created on: 12.10.2015
 *      Author: andreas
 */

#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "Image/Image.h"
#include "Core/Vector.h"
#include "Geometry/Rectangle.h"
namespace fcv {

Vector<int> calcHist(Image* inputImage);
Vector<int> calcHist(Image* inputImage, Rectangle roi);

Vector<int> calcHistCum(Vector<int> hist);

bool equalizeHistogram(Image* inputImage, Image* outputImage);
bool equalizeHistogram(Image* inputImage, Image* outputImage, Rectangle roi);

Image convertHistToImage(Vector<int>* hist, int height = 100);

} /* namespace fcv */

#endif /* HISTOGRAM_H_ */
