/*
 * ColorConvert.h
 *
 *  Created on: 01.11.2015
 *      Author: andreas
 */

#ifndef COLORCONVERT_H_
#define COLORCONVERT_H_

#include "Image/Image.h"
namespace fcv {

typedef enum {
	GRAY8_TO_RGB_888, RGB_888_TO_GRAY8, YUYV_TO_RGB_888, FLOAT_TO_GRAY, GRAY_TO_FLOAT
} PixelFormatConvert;

bool convertPxFormat(Image* input, Image* output, PixelFormatConvert mode);

} /* namespace fcv */

#endif /* COLORCONVERT_H_ */
