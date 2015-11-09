/*
 * testMain.cpp
 *
 *  Created on: 09.10.2015
 *      Author: andreas
 */

#include "FreeCV/FreeCV.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char **argv) {

//	fcv::Image img3 = fcv::ImageFileManager::loadImage("lena1.pgm");
//	fcv::Image img5 = fcv::ImageFileManager::loadImage("equalized.pgm");
//	fcv::ImageFileManager::saveImage(&img5,"equalized2.pgm");
//	if(!img3.isValid())
//	{
//		LOG_ERROR("Can't load image!");
//		return -1;
//	}
//
//	fcv::Image img4(img3,true);
//
//	fcv::filterMedian(&img3,&img4,3);
//	fcv::ImageFileManager::saveImage(&img4,"median.pgm");
//	fcv::filterGauss(&img3,&img4,5);
//	fcv::ImageFileManager::saveImage(&img4,"gaus.pgm");
//
//	fcv::Vectori hist = fcv::calcHist(&img3, fcv::Rectangle(200,200,200,200));
//	fcv::Image histImg = fcv::convertHistToImage(&hist);
//	fcv::ImageFileManager::saveImage(&histImg,"hist.pgm");
//	fcv::Vectori histCum = fcv::calcHistCum(hist);
//	fcv::Image histImgCum = fcv::convertHistToImage(&histCum);
//	fcv::ImageFileManager::saveImage(&histImgCum,"histCum.pgm");
//
//	fcv::filterGauss(&img3,&img3,3);
////	fcv::applyKernel(&img3,&img3,kernel,16);
//	fcv::equalizeHistogram(&img3,&img3);
//	fcv::ImageFileManager::saveImage(&img3,"equalized.pgm");
//
//
//	fcv::Image img6 = fcv::ImageFileManager::loadImage("linienUndKreise.pgm");
//	fcv::imageHoughTransform(&img6,&img3);
//	fcv::ImageFileManager::saveImage(&img3,"hough.pgm",fcv::ImageFileManager::PGM_ASCII);

	fcv::Image imgL = fcv::ImageFileManager::loadImage("stereo_left_gray.pgm");
	fcv::Image imgLColor = fcv::ImageFileManager::loadImage("stereo_left_rgb.ppm");
	fcv::Image imgR = fcv::ImageFileManager::loadImage("stereo_right_gray.pgm");

	assert(imgL.isValid());
	assert(imgLColor.isValid());
	assert(imgR.isValid());

	fcv::SGM sgm;
	fcv::PointCloudCreator pcc;

	sgm.init(imgL.getWidth(), imgL.getHeight(), 60);
	sgm.updatePenalties(25, 250);
	double memUsage = sgm.calcExpectedMemoryUsage();

	std::cout << "Expected Memory Usage: " << memUsage / (1024*1024) << " MegaByte" << std::endl;

	fcv::Vector2f c;
	c[0] = imgL.getWidth()/2;
	c[1] = imgL.getHeight()/2;
	pcc.init(c,150,0.17);

	fcv::Image imgDispL(imgL.getWidth(), imgL.getHeight(), fcv::Image::PF_FLOAT_32);
	fcv::Image imgDispR(imgL.getWidth(), imgL.getHeight(), fcv::Image::PF_FLOAT_32);

	clock_t time = clock();

	sgm.processImagePair(&imgL, &imgR);
	memcpy(imgDispL.getPtr<float>(), sgm.getDisparityData(), imgL.getWidth() * imgL.getHeight() * sizeof(float));

	sgm.processImagePair(&imgL, &imgR, false);
	memcpy(imgDispR.getPtr<float>(), sgm.getDisparityData(), imgL.getWidth() * imgL.getHeight() * sizeof(float));

	std::cout << (float) (clock() - time) * 1000 / CLOCKS_PER_SEC << " ms" << std::endl;

	fcv::SGM::l2rConsistencyCheck(&imgDispL, &imgDispR, 1);

	fcv::PointCloudCreator::PointCloud points;
	pcc.convertDisparity(&imgDispL, &imgLColor, &points, 10);
	fcv::PointCloudCreator::saveToPly("data.ply", &points);
//
	fcv::Image imgDispColor;
//	fcv::convertPxFormat(&imgDispL, &imgDisp8Bit, fcv::FLOAT_TO_GRAY);
//			fcv::filterMedian(&imgDisp8Bit,&imgDisp8Bit,3);
	fcv::convertToPseudoColor(&imgDispL, &imgDispColor, 0 , 60, 0, 120);

	fcv::ImageFileManager::saveImage(&imgDispColor, "disparityL.ppm",
			fcv::ImageFileManager::PPM_BINARY);

	fcv::convertToPseudoColor(&imgDispR, &imgDispColor, 0 , 60, 0, 120);

	fcv::ImageFileManager::saveImage(&imgDispColor, "disparityR.ppm",
			fcv::ImageFileManager::PPM_BINARY);


//	fcv::PointCloudCreator::PointCloud points;
//	fcv::PointCloudCreator::loadFromPly("data.ply",&points);
//	fcv::PointCloudCreator::saveToPly("data.ply",&points);

//	fcv::VideoCapture vc;
//	vc.openAndInitDev("/dev/video0");
//	vc.startCapture();
//	usleep(500000);
//	fcv::Image imgRaw, imgRGB;
//	if(vc.grabFrame(&imgRaw)){
//
//		fcv::convertPxFormat(&imgRaw,&imgRGB,fcv::YUYV_TO_RGB_888);
//
//		fcv::ImageFileManager::saveImage(&imgRGB, "test.ppm",
//				fcv::ImageFileManager::PPM_BINARY);
//	}
//	else
//	{
//		std::cout << "no Data" << std::endl;
//	}
}

