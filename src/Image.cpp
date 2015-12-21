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
Image::Image(Rectangle roi, Image* orgImg)
{
	initEmpty();
	crop(roi, orgImg);
}
Image& Image::operator=(const Image& img)
{
	releaseData();
	if(img.isValid())
		init(img.getWidth(),img.getHeight(),img.getFormat(),img.getPtr<unsigned char>(),true);

	return *this;
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
	m_bytesPerPixel = 0;
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
		m_bytesPerPixel = 1;
		break;
	case PF_RGB_888:
		buffSize = width * height * 3;
		m_bytesPerPixel = 3;
		break;
	case PF_FLOAT_32:
		buffSize = width * height * sizeof(float);
		m_bytesPerPixel = sizeof(float);
		break;
	case PF_YUYV:
		buffSize = width * height * 2;
		m_bytesPerPixel = 2;
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
void Image::crop(Rectangle roi, Image* orgImg) {

	// Crop from own image
	if (orgImg == NULL && isValid()) {
		assert(roi.getTop() >= 0 && roi.getBottom() <= getHeight()
				&& roi.getLeft() >= 0 && roi.getRight() <= getWidth());
		// Temp buffer
		unsigned char* newBuff = new unsigned char[(int)(roi.getWidth()	* roi.getHeight() * getBytesPerPixel())];
		unsigned char* newBuffPtr = newBuff;

		LOG_DEBUG("Crop from own buffer");
		for (int h = roi.getTop(); h < roi.getBottom(); h++) {
			memcpy(newBuffPtr, getPtr<unsigned char>(h,roi.getLeft()),
					getBytesPerPixel() * roi.getWidth());

			newBuffPtr += (int)(getBytesPerPixel() * roi.getWidth());
		}
		if (m_ownMem)
			releaseData();

		m_data = newBuff;
		m_ownMem = true;
		m_width = roi.getWidth();
		m_height = roi.getHeight();

	}
	// Crop from other image
	else if(orgImg != NULL){
		assert(	roi.getTop() >= 0 && roi.getBottom() <= orgImg->getHeight()
				&& roi.getLeft() >= 0
				&& roi.getRight() <= orgImg->getWidth());

		init(roi.getWidth(), roi.getHeight(), orgImg->getFormat());
		for (int h = roi.getTop(); h < roi.getBottom(); h++) {
			memcpy(getPtr<unsigned char>(h), orgImg->getPtr<unsigned char>(h,roi.getLeft()),	orgImg->getBytesPerPixel() * roi.getWidth());
		}
	}else{
		LOG_ERROR("Can't crop from my own buffer. Not initialized!");
	}

}

#ifdef QT_FOUND
	QImage Image::toQImage()
	{
		QImage::Format format;
		switch (m_format) {
		case PF_FLOAT_32:
			LOG_ERROR("Qt does not support float images!");
			return QImage();
			break;
		case PF_RGB_888:
			format = QImage::Format_RGB888;
			break;
		case PF_GRAYSCALE_8:
			format = QImage::Format_Grayscale8;
			break;
		case PF_YUYV:
			LOG_ERROR("Qt does not support YUYV images!");
			return QImage();
			break;
		default:
			return QImage();
			break;
		}
		return QImage(m_data, m_width, m_height, m_bytesPerPixel*m_width,format);
	}
#endif
//Image& Image::operator= (const Image& other){
//    if (this != &other) {
//    	releaseData();
//    	init(other.getWidth(),other.getHeight(),other.getFormat(),other.getPtr<unsigned char>(),true);
//    }
//    return *this;
//}
} /* namespace fcv */
