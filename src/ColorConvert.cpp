/*
 * ColorConvert.cpp
 *
 *  Created on: 01.11.2015
 *      Author: andreas
 */

#include <Image/ColorConvert.h>

namespace fcv {

#define CLIP(X)	X > 255? 255: X<0? 0 : (int) X

bool convertGRAY_RGB(Image* input, Image* output);
bool convertRGB_GRAY(Image* input, Image* output);
bool convertYUYV_RGB(Image* input, Image* output);
bool convertFLOAT_GRAY(Image* input, Image* output);
bool convertGRAY_FLOAT(Image* input, Image* output);

bool convertPxFormat(Image* input, Image* output, PixelFormatConvert mode)
{
	switch (mode) {
		case GRAY8_TO_RGB_888:
			return convertGRAY_RGB(input,output);
			break;
		case RGB_888_TO_GRAY8:
			return convertRGB_GRAY(input,output);
			break;
		case YUYV_TO_RGB_888:
			return convertYUYV_RGB(input,output);
			break;
		case FLOAT_TO_GRAY:
			return convertFLOAT_GRAY(input,output);
			break;
		case GRAY_TO_FLOAT:
			return convertGRAY_FLOAT(input,output);
			break;
		default:
			break;
	}
	return false;
}
bool convertGRAY_RGB(Image* input, Image* output)
{
	output->init(input->getWidth(),input->getHeight(),Image::PF_RGB_888);

	unsigned char* inputPtr = input->getPtr<unsigned char>();
	unsigned char* outputPtr = output->getPtr<unsigned char>();

	for(int i = 0; i < input->getWidth()*input->getHeight(); i++){

		*outputPtr++ = *inputPtr;
		*outputPtr++ = *inputPtr;
		*outputPtr++ = *inputPtr++;
	}
	return true;
}
bool convertRGB_GRAY(Image* input, Image* output)
{
	output->init(input->getWidth(),input->getHeight(),Image::PF_GRAYSCALE_8);

	unsigned char* inputPtr = input->getPtr<unsigned char>();
	unsigned char* outputPtr = output->getPtr<unsigned char>();

	for(int i = 0; i < input->getWidth()*input->getHeight(); i++){

		*outputPtr++ = *inputPtr++ * 0.144
				+ *inputPtr++ * 0.587
				+ *inputPtr++ * 0.299;
	}
	return true;
}
bool convertYUYV_RGB(Image* input, Image* output)
{
	output->init(input->getWidth(), input->getHeight(), Image::PF_RGB_888);

	unsigned char* inputPtr = input->getPtr<unsigned char>();
	unsigned char* outputPtr = output->getPtr<unsigned char>();

	for (int i = 0; i < input->getWidth() * input->getHeight(); i+=2) {
		int u, v, y1, y2;

		y1 = *inputPtr++;
		u = *inputPtr++;
		y2 = *inputPtr++;
		v = *inputPtr++;

		// Integer operation of ITU-R standard for YCbCr is (from Wikipedia)
		// https://en.wikipedia.org/wiki/YUV#Y.27UV422_to_RGB888_conversion
		u = u - 128;
		v = v - 128;

		*outputPtr++ = CLIP(y1 + 45 * v / 32);
		*outputPtr++ = CLIP(y1 - (11 * u + 23 * v) / 32);
		*outputPtr++ = CLIP(y1 + 113 * u / 64);

		*outputPtr++ = CLIP(y2 + 45 * v / 32);
		*outputPtr++ = CLIP(y2 - (11 * u + 23 * v) / 32);
		*outputPtr++ = CLIP(y2 + 113 * u / 64);
	}
	return true;
}
bool convertFLOAT_GRAY(Image* input, Image* output)
{
	output->init(input->getWidth(), input->getHeight(), Image::PF_GRAYSCALE_8);

	float* inputPtr = input->getPtr<float>();
	unsigned char* outputPtr = output->getPtr<unsigned char>();

	for (int i = 0; i < input->getWidth() * input->getHeight(); i++) {
		*outputPtr++ = *inputPtr++;
	}
	return true;
}
bool convertGRAY_FLOAT(Image* input, Image* output)
{
	output->init(input->getWidth(), input->getHeight(), Image::PF_FLOAT_32);

	unsigned char* inputPtr = input->getPtr<unsigned char>();
	float* outputPtr = output->getPtr<float>();

	for (int i = 0; i < input->getWidth() * input->getHeight(); i++) {
		*outputPtr++ = *inputPtr++;
	}
	return true;
}

} /* namespace fcv */
