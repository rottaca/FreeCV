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

#include "Core/Logger.h"
#include "Image/Image.h"
namespace fcv {

class ImageFileManager {
public:
	static Image loadImage(std::string fileName);
	static bool saveImage(Image* image, std::string fileName);
};

} /* namespace fcv */

#endif /* IMAGEFILEMANAGER_H_ */
