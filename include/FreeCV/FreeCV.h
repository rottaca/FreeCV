/*
 * freecv.h
 *
 *  Created on: 13.10.2015
 *      Author: andreas
 */

#ifndef FREECV_H_
#define FREECV_H_

#include "FreeCV/Core/Logger.h"
#include "FreeCV/Core/StopWatch.h"
#include "FreeCV/Core/Macros.h"
#include "FreeCV/Core/Matrix.h"
#include "FreeCV/Core/Vector.h"
#include "FreeCV/Core/Rectangle.h"
#include "FreeCV/Core/Math.h"

#include "FreeCV/Image/Filter.h"
#include "FreeCV/Image/Histogram.h"
#include "FreeCV/Image/Image.h"
#include "FreeCV/Image/ImageFileManager.h"
#include "FreeCV/Image/Transform.h"
#include "FreeCV/Image/ColorConvert.h"

#include "FreeCV/Stereo/SGM.h"
#include "FreeCV/Stereo/PointCloudCreator.h"
#include "FreeCV/Stereo/UndistortAndRectify.h"

#include "FreeCV/Neuro/IMLPTransfer.h"
#include "FreeCV/Neuro/MLPTransferTanh.h"
#include "FreeCV/Neuro/MLPRProp.h"

// Linux headers
#ifdef __linux__
	#include "FreeCV/Linux/VideoCapture.h"
	#include "FreeCV/Linux/ExposureController.h"
#endif

// Windows headers
#ifdef _WIN32
	#include "FreeCV/Windows/times.h"
#endif

#endif /* FREECV_H_ */
