/*
 * Image.cpp
 *
 *  Created on: 09.10.2015
 *      Author: andreas
 */

#include "Image/Image.h"

namespace fcv {

Image::Image() {
	m_width = 0;
	m_height = 0;
	m_data = NULL;
	m_ownMem = false;
	m_format = PF_GRAYSCALE_8;
}
Image::Image(const Image& img)
{
	init(img.getWidth(),img.getHeight(),img.getFormat(),img.getPtr<unsigned char>(),true);
}
Image::Image(Image img, bool copyData)
{
	init(img.getWidth(),img.getHeight(),img.getFormat(),img.getPtr<unsigned char>(),copyData);
}
Image::Image(int width, int height, PixelFormat f)
{
	init(width,height,f);
}
Image::Image(int width, int height, PixelFormat f, unsigned char* data, bool copyData)
{
	init(width,height,f,data,copyData);
}

Image::~Image() {
	releaseData();
}
void Image::init(int width, int height, PixelFormat f, unsigned char* data, bool copyData)
{
	m_width = width;
	m_height = height;
	m_format = f;

	if (data != NULL) {
		m_ownMem = copyData;
		if (!copyData){
			m_data = data;
		}
		else {
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
			default:
				m_data = NULL;
				buffSize = 0;
				m_channels = 0;
				return;
			}
			m_data = new unsigned char[buffSize];
			memcpy(m_data, data, buffSize);
		}
	} else {
		m_ownMem = true;
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
		default:
			m_channels = 0;
			m_data = NULL;
			return;
		}
		m_data = new unsigned char[buffSize];
	}
}

void Image::releaseData(){
	if (m_ownMem && m_data != NULL) {
		delete[] m_data;
		m_data = NULL;
		m_ownMem = false;
	}
}
Image& Image::operator= (const Image& other){
    if (this != &other) {
    	releaseData();
    	init(other.getWidth(),other.getHeight(),other.getFormat(),other.getPtr<unsigned char>(),true);
    }
    return *this;
}
} /* namespace fcv */
