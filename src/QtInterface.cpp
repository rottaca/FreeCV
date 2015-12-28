/*
 * QtInterface.cpp
 *
 *  Created on: 28.12.2015
 *      Author: andreas
 */

#include <FreeCV/Interfaces/QtInterface.h>
#include <FreeCV/Core/Logger.h>

namespace fcv {

QImage toQImage(Image& img)
{
		QImage::Format format;
		switch (img.getFormat()) {
		case Image::PF_FLOAT_32:
			LOG_ERROR("Qt does not support float images!");
			return QImage();
			break;
		case Image::PF_RGB_888:
			format = QImage::Format_RGB888;
			break;
		case Image::PF_GRAYSCALE_8:
			format = QImage::Format_Grayscale8;
			break;
		case Image::PF_YUYV:
			LOG_ERROR("Qt does not support YUYV images!");
			return QImage();
			break;
		default:
			return QImage();
			break;
		}
		return QImage(img.getPtr<unsigned char>(), img.getWidth(), img.getHeight(), img.getBytesPerPixel()*img.getWidth(),format);
	}
} /* namespace fcv */
