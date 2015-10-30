/*
 * Transform.cpp
 *
 *  Created on: 13.10.2015
 *      Author: andreas
 */

#include "Image/Transform.h"
#include <math.h>
#include <algorithm>

namespace fcv {

bool imagePositionAltitudeTransform(Image* inputImage, Image* outputImage,
		float angle, Vector2f t, Vector2f s) {
	// TODO
}

bool imageHoughTransform(Image* inputBinaryImage, Image* outputHoughImage) {
	int width = inputBinaryImage->getWidth();
	int height = inputBinaryImage->getHeight();

	int maxR = ((sqrt(2.0) * (double)(height>width?height:width)) / 2.0);

	int* accu = new int[maxR * 2 * 180];
	memset(accu,0,sizeof(int)*maxR*2*180);

	unsigned char* inputPtr = inputBinaryImage->getPtr<unsigned char>();

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if(*inputPtr > 250)
			{
				for(int t = 0; t < 180 ; t++)
				{
					float r = (x - width/2)*cos((double)t*180.0 / M_PI);
					r += (y - height/2)*sin((double)t*180.0 / M_PI);
					accu[int(r + maxR*2) + maxR * 2 * t]++;
				}
			}
			inputPtr++;
		}
	}

	outputHoughImage->init(180, maxR * 2, Image::PF_GRAYSCALE_8);

	int max = *std::max_element(accu,accu + maxR*2*180);
	int* accuPtr = accu;
	unsigned char* outputImgPtr = outputHoughImage->getPtr<unsigned char>();
	for(int r = 0; r < maxR*2; r++){
		for(int t = 0; t < 180; t++){
			*outputImgPtr++ = *accuPtr++*255.0/max;
		}
	}
	delete[] accu;
	return true;
}
} /* namespace fcv */
