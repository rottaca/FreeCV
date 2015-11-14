/*
 * Image.h
 *
 *  Created on: 09.10.2015
 *      Author: andreas
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include "string.h"
#include "sstream"
#include <typeinfo>
#include "assert.h"

#include "FreeCV/Core/Logger.h"
#include "FreeCV/Core/Rectangle.h"

namespace fcv {

class Image {
public:
	typedef enum {
		PF_GRAYSCALE_8, PF_RGB_888, PF_FLOAT_32, PF_YUYV
	} PixelFormat;

	Image();
	Image(const Image& img);
	Image(Image img, bool copyData);
	Image(int width, int height, PixelFormat f);
	Image(int width, int height, PixelFormat f, unsigned char* data,
			bool copyData);
	Image(Rectangle roi, Image* orgImg);

	virtual ~Image();

	Image clone() {
		if (m_data == NULL)
			return Image();
		else
			return Image(m_width, m_height, m_format, m_data, true);
	}

	template<typename T>
	inline T* getPtr() const {
		return (T*) m_data;
	}
	template<typename T>
	inline T* getPtr(int y) const {
		assert(y < m_height && y >= 0);
		return (T*)(m_data + y * m_width * m_bytesPerPixel);
	}
	template<typename T>
	inline T* getPtr(int y, int x) const {
		assert(y < m_height && x < m_width && y >= 0 && x >= 0);
		return (T*)(m_data + y * m_width  * m_bytesPerPixel + x * m_bytesPerPixel);
	}
	template<typename T>
	T& at(int y, int x) {
		assert(y < m_height && x < m_width && y >= 0 && x >= 0);
		return *getPtr<T>(y, x);
	}

	Image& operator=(const Image& img);

	int getWidth() const {
		return m_width;
	}
	;
	int getHeight() const {
		return m_height;
	}
	int getBytesPerPixel() const {
		return m_bytesPerPixel;
	}
	;
	PixelFormat getFormat() const {
		return m_format;
	}
	;

	bool ownsMem() const {
		return m_ownMem;
	}
	bool isValid() const {
		return m_width != 0 && m_height != 0 && m_data != NULL;
	}
	std::string toString() const {
		std::stringstream s;
		s << "Image: \nType: ";

		switch (m_format) {
		case PF_GRAYSCALE_8:
			s << "GRAYSCALE_8";
			break;
		case PF_RGB_888:
			s << "RGB_888";
			break;
		case PF_FLOAT_32:
			s << "FLOAT_32";
			break;
		case PF_YUYV:
			s << "YUYV";
			break;
		default:
			s << "Unknown";
		}
		if (!isValid()) {
			s << "\nInvalid Image!";
		}
		s << "\nSize: " << m_width << "x" << m_height;
		s << "\nOwnsBuffer: " << m_ownMem;
		return s.str();
	}

	bool init(int width, int height, PixelFormat f, unsigned char* data = NULL,
			bool copyData = false);

	void initEmpty();

	void crop(Rectangle roi, Image* orgImg = NULL);

private:
	int m_width, m_height, m_bytesPerPixel;
	bool m_ownMem;
	unsigned char* m_data;
	PixelFormat m_format;

	void releaseData();
};

} /* namespace fcv */

#endif /* IMAGE_H_ */
