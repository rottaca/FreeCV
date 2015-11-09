/*
 * ImageFileManager.h
 *
 *  Created on: 09.10.2015
 *      Author: andreas
 */

#ifndef IMAGEFILEMANAGER_H_
#define IMAGEFILEMANAGER_H_

#include "string.h"
#include "sstream"
#include <typeinfo>

#include "FreeCV/Image/Image.h"
namespace fcv {

class ImageFileManager {
public:
	typedef enum {UNKNOWN, PGM_ASCII, PGM_BINARY, PPM_ASCII, PPM_BINARY} ImageFileType;

	static Image loadImage(std::string fileName, ImageFileType type = UNKNOWN);
	static bool saveImage(Image* image, std::string fileName, ImageFileType type = UNKNOWN);
};

} /* namespace fcv */

#endif /* IMAGEFILEMANAGER_H_ */
