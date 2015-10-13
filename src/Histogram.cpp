/*
 * Histogram.cpp
 *
 *  Created on: 12.10.2015
 *      Author: andreas
 */

#include "Image/Histogram.h"
#include "assert.h"

namespace fcv {

bool equalizeHistogram(Image* inputImage, Image* outputImage)
{
	return equalizeHistogram(inputImage,outputImage,Rectangle(0,0,inputImage->getWidth(),inputImage->getHeight()));
}
bool equalizeHistogram(Image* inputImage, Image* outputImage, Rectangle roi)
{
	assert(inputImage->getFormat() == Image::PF_GRAYSCALE_8);

	Vector<int> hist = calcHist(inputImage);

    // Caluculate the size of image
    int size = inputImage->getWidth() * inputImage->getHeight();
    float alpha = 255.0/size;

    // Calculate the probability of each intensity
    Vector<float> PrRk(256);
    for(int i = 0; i < 256; i++)
    {
        PrRk[i] = (float)hist[i] / size;
	}

	// Generate cumulative frequency histogram
    Vector<int> cumhistogram = calcHistCum(hist);
    unsigned char* outPtr = outputImage->getPtr<unsigned char>();
    unsigned char* inPtr = inputImage->getPtr<unsigned char>();
    for(int i = 0; i < size; i++){
    	*outPtr++ = cumhistogram[*inPtr++] * alpha;
    }

	return true;
}
Vector<int> calcHistCum(Vector<int> hist)
{
	Vector<int> histcum(hist.getSize());
	histcum[0] = hist[0];

	for (int i = 1; i < hist.getSize(); i++) {
		histcum[i] = hist[i] + histcum[i - 1];
	}
	return histcum;
}
Vector<int> calcHist(Image* inputImage)
{
	return calcHist(inputImage,Rectangle(0,0,inputImage->getWidth(),inputImage->getHeight()));
}
Vector<int> calcHist(Image* inputImage, Rectangle roi)
{
	assert(inputImage->getFormat() == Image::PF_GRAYSCALE_8);
	assert(roi.getTop() >= 0 && roi.getLeft() >= 0
					&& roi.getRight() <= inputImage->getWidth()
					&& roi.getBottom() <= inputImage->getHeight());

	Vector<int> hist(256);
	int offset = inputImage->getWidth() - roi.getRight() + roi.getLeft();
	unsigned char* ptr = inputImage->getPtr<unsigned char>()
			+ (int) roi.getLeft();

	for (int y = roi.getTop(); y < roi.getBottom(); y++) {
		for (int x = roi.getLeft(); x < roi.getRight(); x++) {
			hist[*ptr++]++;
		}
	}
	return hist;
}

Image convertHistToImage(Vector<int>* hist, int height){
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
