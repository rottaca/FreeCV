/*
 * Filter.cpp
 *
 *  Created on: 11.10.2015
 *      Author: andreas
 */

#include "Image/Filter.h"
#include "math.h"

namespace fcv {

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

bool applyKernel(Image* inputImage, Image* outputImage, Matrix<int>& kernel,
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
