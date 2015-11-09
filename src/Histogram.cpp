/*
 * Histogram.cpp
 *
 *  Created on: 12.10.2015
 *      Author: andreas
 */

#include "FreeCV/Image/Histogram.h"
#include "assert.h"

namespace fcv {

bool equalizeHistogram(Image* inputImage, Image* outputImage)
{
	return equalizeHistogram(inputImage,outputImage,Rectangle(0,0,inputImage->getWidth(),inputImage->getHeight()));
}
bool equalizeHistogram(Image* inputImage, Image* outputImage, Rectangle roi)
{
	assert(inputImage->getFormat() == Image::PF_GRAYSCALE_8);

	Vectori hist = calcHist(inputImage, roi);

    // Caluculate the size of image
    int size = inputImage->getWidth() * inputImage->getHeight();
    float alpha = 255.0/size;

	// Generate cumulative frequency histogram
    Vectori cumhistogram = calcHistCum(hist);
    unsigned char* outPtr = outputImage->getPtr<unsigned char>(roi.getTop(),roi.getLeft());
    unsigned char* inPtr = inputImage->getPtr<unsigned char>(roi.getTop(),roi.getLeft());

    // Remap image
	int offset = inputImage->getWidth() - roi.getRight() + roi.getLeft();
	for (int y = roi.getTop(); y < roi.getBottom(); y++) {
		for (int x = roi.getLeft(); x < roi.getRight(); x++) {
			*outPtr++ = cumhistogram[*inPtr++] * alpha;
		}
		outPtr += offset;
		inPtr += offset;
	}

	return true;
}
Vectori calcHistCum(Vectori hist)
{
	Vectori histcum(hist.getSize());
	histcum[0] = hist[0];

	for (int i = 1; i < hist.getSize(); i++) {
		histcum[i] = hist[i] + histcum[i - 1];
	}
	return histcum;
}
Vectori calcHist(Image* inputImage)
{
	return calcHist(inputImage,Rectangle(0,0,inputImage->getWidth(),inputImage->getHeight()));
}
Vectori calcHist(Image* inputImage, Rectangle roi)
{
	assert(inputImage->getFormat() == Image::PF_GRAYSCALE_8);
	assert(roi.getTop() >= 0 && roi.getLeft() >= 0
					&& roi.getRight() <= inputImage->getWidth()
					&& roi.getBottom() <= inputImage->getHeight());

	Vectori hist(256);
	int offset = inputImage->getWidth() - roi.getRight() + roi.getLeft();
	unsigned char* ptr = inputImage->getPtr<unsigned char>(roi.getTop(),roi.getLeft());

	for (int y = roi.getTop(); y < roi.getBottom(); y++) {
		for (int x = roi.getLeft(); x < roi.getRight(); x++) {
			hist[*ptr++]++;
		}
		ptr += offset;
	}
	return hist;
}

Image convertHistToImage(Vectori* hist, int height){
	int width = 256;
	int dataheight = hist->getMax();

	Image img(width, height, Image::PF_GRAYSCALE_8);

	unsigned char* ptr = img.getPtr<unsigned char>();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if((float)(*hist)[x]/dataheight >= (float)(height- y) / height )
				*ptr++ = 0;
			else
				*ptr++ = 255;
		}
	}

	return img;
}

} /* namespace fcv */
