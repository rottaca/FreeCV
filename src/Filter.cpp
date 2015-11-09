/*
 * Filter.cpp
 *
 *  Created on: 11.10.2015
 *      Author: andreas
 */

#include "FreeCV/Image/Filter.h"
#include "math.h"

namespace fcv {

bool filterGauss(Image* inputImage, Image* outputImage, int kernelSize) {

	Matrixi kernel(kernelSize,kernelSize);
	int weightSum;

	switch (kernelSize) {
	case 3:
		kernel.at(0,0) = 1;
		kernel.at(0,1) = 2;
		kernel.at(0,2) = 1;

		kernel.at(1,0) = 2;
		kernel.at(1,1) = 4;
		kernel.at(1,2) = 2;

		kernel.at(2,0) = 1;
		kernel.at(2,1) = 2;
		kernel.at(2,2) = 1;

		weightSum = 16;
		break;
	case 5:
		kernel.at(0,0) = 1;
		kernel.at(0,1) = 4;
		kernel.at(0,2) = 6;
		kernel.at(0,3) = 4;
		kernel.at(0,4) = 1;

		kernel.at(1,0) = 4;
		kernel.at(1,1) = 16;
		kernel.at(1,2) = 24;
		kernel.at(1,3) = 16;
		kernel.at(1,4) = 4;

		kernel.at(2,0) = 6;
		kernel.at(2,1) = 24;
		kernel.at(2,2) = 36;
		kernel.at(2,3) = 24;
		kernel.at(2,4) = 6;

		kernel.at(3,0) = 4;
		kernel.at(3,1) = 16;
		kernel.at(3,2) = 24;
		kernel.at(3,3) = 16;
		kernel.at(3,4) = 4;

		kernel.at(4,0) = 1;
		kernel.at(4,1) = 4;
		kernel.at(4,2) = 6;
		kernel.at(4,3) = 4;
		kernel.at(4,4) = 1;

		weightSum = 256;

		break;
	default:
		// TODO Generisch!
		return false;
		break;
	}
	applyKernel(inputImage,outputImage,kernel,weightSum);

	return true;
}
bool filterMedian(Image* inputImage, Image* outputImage, int kernelSize) {
	assert(kernelSize % 2 == 1);
	assert(inputImage->getFormat() == outputImage->getFormat());

	int halfKernelSize = kernelSize / 2;
	int windowSize = kernelSize * kernelSize;
	int width = inputImage->getWidth();
	int height = inputImage->getHeight();

	switch (inputImage->getFormat()) {
	case Image::PF_GRAYSCALE_8: {
		unsigned char* outImgPtr = outputImage->getPtr<unsigned char>();
		unsigned char window[windowSize];
		int xTmp, yTmp;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {

				memset(window, 0, windowSize * sizeof(unsigned char));
				int dataIdx = 0;

				// TODO Improve code
				for (int ky = -halfKernelSize; ky <= halfKernelSize; ky++) {
					yTmp = abs(y + ky);
					if (yTmp >= height)
						yTmp = height - 1 - (yTmp - height);

					unsigned char* inImgPtr =
							inputImage->getPtr<unsigned char>() + yTmp * width;
					for (int kx = -halfKernelSize; kx <= halfKernelSize; kx++) {

						xTmp = abs(x + kx);
						if (xTmp >= width)
							xTmp = width - 1 - (xTmp - width);

						window[dataIdx++] = *(inImgPtr + xTmp);
					}
				}

				for (int i = 0; i <= windowSize / 2; i++) {
					//   Find position of minimum element
					int min = i;
					for (int l = i + 1; l < windowSize; ++l)
						if (window[l] < window[min])
							min = l;
					//   Put found minimum element in its place
					unsigned char temp = window[i];
					window[i] = window[min];
					window[min] = temp;
				}

				*outImgPtr++ = window[windowSize / 2];
			}
		}
		break;
	}
	default:
		LOG_ERROR("Invalid Pixelformat. Format not supported!");
		return false;
	}
	return true;
}

bool applyKernel(Image* inputImage, Image* outputImage, Matrixi& kernel,
		int weightSum) {
	assert(kernel.getCols() == kernel.getRows());
	assert(kernel.getCols() % 2 == 1);

	int halfKernelSize = kernel.getCols() / 2;
	int width = inputImage->getWidth();
	int height = inputImage->getHeight();

	int pixelVal = 0;

	switch (inputImage->getFormat()) {
	case Image::PF_GRAYSCALE_8: {
		unsigned char* outImgPtr = outputImage->getPtr<unsigned char>();
		int xTmp, yTmp;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {

				pixelVal = 0;
				int* kernelPtr = kernel.getPtr() - 1;

				for (int ky = -halfKernelSize; ky <= halfKernelSize; ky++) {
					yTmp = abs(y + ky);
					if (yTmp >= height)
						yTmp = height - 1 - (yTmp - height);
					unsigned char* inImgPtr =
							inputImage->getPtr<unsigned char>() + yTmp * width;
					for (int kx = -halfKernelSize; kx <= halfKernelSize; kx++) {

						xTmp = abs(x + kx);
						if (xTmp >= width)
							xTmp = width - 1 - (xTmp - width);

						kernelPtr++;
						pixelVal += (*kernelPtr) * *(inImgPtr + xTmp);

					}
				}
				*outImgPtr++ = clip((int) (pixelVal / weightSum), 0, 255);
			}
		}
		break;
	}
	default:
		LOG_ERROR("Invalid Pixelformat. Format not supported!");
		return false;
	}
	return true;
}

} /* namespace fcv */
