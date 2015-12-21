#ifndef GRABBER_H_
#define GRABBER_H_

#include <string>
#include <vector>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <iostream>
#include "FreeCV/Image/Image.h"

namespace fcv {

class VideoCapture {
public:
	VideoCapture();
	~VideoCapture();

	typedef struct CaptureDeviceInfo{
		std::string devName;
	} CaptureDeviceInfo;
	static std::vector<CaptureDeviceInfo> enumDevices();

	bool openAndInitDev(std::string devStr, int reqWidth = -1, int reqHeight = -1);
	void closeDev();

	bool startCapture();
	bool stopCapture();

	int getWidth();
	int getHeight();

	bool grabFrame(Image* frame);

	void printCaps();

	bool setAutoexposureEnabled(bool enabled, bool dropFPS);
	bool setExposureTime(unsigned int time_us);

	bool isCaptureing()
	{
		return m_isCaptureing;
	}

private:
	struct buffer {
	        void   *start;
	        size_t  length;
	};

	struct buffer* buffers;
	unsigned int   n_buffers;

	std::string devStr;
	bool m_isCaptureing;
	bool m_isInitialized;

	int devFd;

	Image::PixelFormat fcvFormat;
	struct v4l2_pix_format format;


};
}

#endif
