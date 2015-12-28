/*
 * QtInterface.h
 *
 *  Created on: 28.12.2015
 *      Author: andreas
 */

#ifndef FREECV_INCLUDE_FREECV_QTINTERFACE_H_
#define FREECV_INCLUDE_FREECV_QTINTERFACE_H_

#include <QtGui/qimage.h>

#include <FreeCV/Image/Image.h>

namespace fcv {

	QImage toQImage(Image& img);


} /* namespace fcv */

#endif /* FREECV_INCLUDE_FREECV_QTINTERFACE_H_ */
