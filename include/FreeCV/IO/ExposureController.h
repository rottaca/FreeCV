/*
 * ExposureController.h
 *
 *  Created on: 15.11.2015
 *      Author: andreas
 */

#ifndef FREECV_INCLUDE_FREECV_IO_EXPOSURECONTROLLER_H_
#define FREECV_INCLUDE_FREECV_IO_EXPOSURECONTROLLER_H_

#include "FreeCV/IO/VideoCapture.h"
#include "FreeCV/Image/Image.h"
namespace fcv {


class ExposureController {
public:
	ExposureController();
	virtual ~ExposureController();

	typedef enum {AUTOMATIC, MANUAL_CTRL, MANUAL_FIXED} ExposureCtrlMode;

	bool setupExposureControl(VideoCapture* cam, ExposureCtrlMode mode, int goalVal = -1);
	void controlExposure(Image* imgGray);

	void updateGoalVal(int val);

private:
	bool setAutomaticExpCtrl(bool enable);
	bool setExposureValue(int val);

private:
	VideoCapture* m_cam;
	ExposureCtrlMode m_ctrlMode;
	int m_goalVal;

};

} /* namespace fcv */

#endif /* FREECV_INCLUDE_FREECV_IO_EXPOSURECONTROLLER_H_ */
