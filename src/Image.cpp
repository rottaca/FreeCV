/*
 * Image.cpp
 *
 *  Created on: 09.10.2015
 *      Author: andreas
 */

#include "FreeCV/Image/Image.h"
#include "FreeCV/Core/Logger.h"

namespace fcv {

Image::Image() {
	initEmpty();
}
Image::Image(const Image& img)
{
	initEmpty();
	if(img.isValid())
		init(img.getWidth(),img.getHeight(),img.getFormat(),img.getPtr<unsigned char>(),true);
}
Image::Image(Image img, bool copyData)
{
	initEmpty();
	if(img.isValid())
		init(img.getWidth(),img.getHeight(),img.getFormat(),img.getPtr<unsigned char>(),copyData);
}
Image::Image(int width, int height, PixelFormat f)
{
	initEmpty();
	init(width,height,f);
}
Image::Image(int width, int height, PixelFormat f, unsigned char* data, bool copyData)
{
	initEmpty();
	init(width,height,f,data,copyData);
}

Image::~Image() {
	releaseData();
}
void Image::initEmpty()
{
	m_width = 0;
	m_height = 0;
	m_data = NULL;
	m_ownMem = false;
	m_format = PF_GRAYSCALE_8;
	m_channels = 0;
}
bool Image::init(int width, int height, PixelFormat f, unsigned char* data, bool copyData)
{
	bool recreateBuffer = m_width != width ||
			m_height != height ||
			m_format != f ||
			(data != NULL && copyData && !ownsMem());

	m_width = width;
	m_height = height;
	m_format = f;

	size_t buffSize = 0;
	switch (f) {
	case PF_GRAYSCALE_8:
		buffSize = width * height;
		m_channels = 1;
		break;
	case PF_RGB_888:
		buffSize = width * height * 3;
		m_channels = 3;
		break;
	case PF_FLOAT_32:
		buffSize = width * height * sizeof(float);
		m_channels = sizeof(float);
		break;
	case PF_YUYV:
		buffSize = width * height * 2;
		m_channels = 3;
		break;
	default:
		initEmpty();
		LOG_ERROR("Invalid Pixelformat speicifed!");
		return false;
	}

	if(recreateBuffer){
		releaseData();
		m_data = new unsigned char[buffSize];
		m_ownMem = true;
	}

	if (data != NULL) {
		m_ownMem = copyData || recreateBuffer;
		if (!copyData)
			m_data = data;
		else
			memcpy(m_data, data, buffSize);
	}
	return true;
}

void Image::releaseData(){
	if (m_ownMem && m_data != NULL) {
		delete[] m_data;
		m_data = NULL;
		m_ownMem = false;
	}
}
//Image& Image::operator= (const Image& other){
//    if (this != &other) {
//    	releaseData();
//    	init(other.getWidth(),other.getHeight(),other.getFormat(),other.getPtr<unsigned char>(),true);
//    }
//    return *this;
//}
} /* namespace fcv */
