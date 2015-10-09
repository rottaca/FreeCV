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

#include "Logger.h"

namespace fcv {

class Image {
public:
	typedef enum{GRAYSCALE_8, RGB_888, FLOAT_32} PixelFormat;

	Image();
	Image(const Image& img);
	Image(Image img, bool copyData);
	Image(int width, int height, PixelFormat f);
	Image(int width, int height, PixelFormat f, unsigned char* data, bool copyData);
	virtual ~Image();


	Image clone(){
		if(m_data == NULL)
			return Image();
		else
			return Image(m_width,m_height,m_format,m_data,true);
	}

	template<typename T>
	T* getPtr() const {return (T*) m_data;}

	Image& operator=(const Image& img);

	int getWidth() const { return m_width;};
	int getHeight() const { return m_height;};
	PixelFormat getFormat() const { return m_format;};
	int getBitsPerPixel() const {return m_bitsPerPixel;}
	bool ownsMem() const {return m_ownMem;}
	bool isValid() const{return m_width != 0 && m_height != 0 && m_data != NULL;}
	std::string toString() {
		std::stringstream s;
		s << "Image: \nType: ";

		switch (m_format) {
		case GRAYSCALE_8:
			s << "GRAYSCALE_8";
			break;
		case RGB_888:
			s << "RGB_888";
			break;
		case FLOAT_32:
			s << "FLOAT_32";
			break;
		default:
			s << "Unknown";
		}
		if(!isValid()){
			s << "\nInvalid Image!";
		}
		s << "\nSize: " << m_width << "x" << m_height;
		s << "\nOwnsBuffer: " << m_ownMem;
		return s.str();
	}


private:
	int m_width, m_height, m_bitsPerPixel;
	bool m_ownMem;
	unsigned char* m_data;
	PixelFormat m_format;


	void init(int width, int height, PixelFormat f, unsigned char* data = NULL, bool copyData = false);
	void releaseData();
};

} /* namespace fcv */

#endif /* IMAGE_H_ */
