/*
 * ExposureController.cpp
 *
 *  Created on: 15.11.2015
 *      Author: andreas
 */

#include "FreeCV/Linux/ExposureController.h"

namespace fcv {

ExposureController::ExposureController() {
	m_cam = NULL;
	m_ctrlMode = AUTOMATIC;
	m_goalVal = -1;
}

ExposureController::~ExposureController() {

}
bool ExposureController::setupExposureControl(VideoCapture* cam, ExposureCtrlMode mode, int goalVal)
{
	m_cam = cam;

	if(mode != AUTOMATIC && goalVal == -1)
	{
		LOG_ERROR("Please specify exposure ctrl goal, when manual mode is enabled!");
		return false;
	}

	m_ctrlMode = mode;
	m_goalVal = goalVal;

	if(!setAutomaticExpCtrl(mode == AUTOMATIC)){
		LOG_ERROR("Can't switch to automatic/manual exposure mode!");
		return false;
	}

	if(mode == MANUAL_FIXED){
		if (!setExposureValue(goalVal)) {
			LOG_ERROR("Can't set exposure time!");
			return false;
		}
	}
	return true;
}
void ExposureController::controlExposure(Image* imgGray)
{
	assert(imgGray->getFormat() == Image::PF_GRAYSCALE_8);

	switch (m_ctrlMode) {
		case AUTOMATIC:{
			return;}
		case MANUAL_CTRL:{
			unsigned char* imgPtr = imgGray->getPtr<unsigned char>();
			// TODO
			break;}
		case MANUAL_FIXED:{
			return;}
		default:
			break;
	}
}

void ExposureController::updateGoalVal(int val)
{
	switch (m_ctrlMode) {
	case AUTOMATIC: {
		return;
	}
	case MANUAL_CTRL: {
		m_goalVal = val;
	}
		return;
	case MANUAL_FIXED: {
		m_goalVal = val;
		setExposureValue(val);
		return;
	}
	default:
		break;
	}
}
bool ExposureController::setAutomaticExpCtrl(bool enable)
{
	return m_cam->setAutoexposureEnabled(enable, false);
}
bool ExposureController::setExposureValue(int val)
{
	return m_cam->setExposureTime(val);
}
} /* namespace fcv */
