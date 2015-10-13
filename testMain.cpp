/*
 * testMain.cpp
 *
 *  Created on: 09.10.2015
 *      Author: andreas
 */

#include "FreeCV.h"

int main(int argc, char **argv) {

	fcv::Image img3 = fcv::ImageFileManager::loadImage("lena1.pgm");
	if(!img3.isValid())
	{
		LOG_ERROR("Can't load image!");
		return -1;
	}
	fcv::Matrix<int> kernel(3,3);
	kernel.at(0,0) = 1;
	kernel.at(0,1) = 2;
	kernel.at(0,2) = 1;

	kernel.at(1,0) = 2;
	kernel.at(1,1) = 4;
	kernel.at(1,2) = 2;

	kernel.at(2,0) = 1;
	kernel.at(2,1) = 2;
	kernel.at(2,2) = 1;

//	fcv::Matrix<int> kernel2(3,3);
//	kernel2.at(0,0) = 1;
//	kernel2.at(0,1) = 2;
//	kernel2.at(0,2) = 1;
//
//	kernel2.at(1,0) = 2;
//	kernel2.at(1,1) = 4;
//	kernel2.at(1,2) = 2;
//
//	kernel2.at(2,0) = 1;
//	kernel2.at(2,1) = 2;
//	kernel2.at(2,2) = 1;

//	kernel.at(0,0) = 1;
//	kernel.at(0,1) = 2;
//	kernel.at(0,2) = 1;
//
//	kernel.at(1,0) = 2;
//	kernel.at(1,1) = 4;
//	kernel.at(1,2) = 2;
//
//	kernel.at(2,0) = 1;
//	kernel.at(2,1) = 2;
//	kernel.at(2,2) = 1;

	fcv::Image img4(img3,true);
	fcv::applyKernel(&img3,&img4,kernel,16);
//	fcv::applyKernel(&img4,&img4,kernel,16);
//	fcv::applyKernel(&img4,&img4,kernel,16);
//	fcv::filterMedian(&img3,&img4,3);
	fcv::ImageFileManager::saveImage(&img4,"median.pgm");
//	fcv::filterGauss(&img3,&img4,5);
//	fcv::ImageFileManager::saveImage(&img4,"gaus.pgm");


	fcv::Vector<int> hist = fcv::calcHist(&img3, fcv::Rectangle(200,200,200,200));
	fcv::Image histImg = fcv::convertHistToImage(&hist);
	fcv::ImageFileManager::saveImage(&histImg,"hist.pgm");
	fcv::Vector<int> histCum = fcv::calcHistCum(hist);
	fcv::Image histImgCum = fcv::convertHistToImage(&histCum);
	fcv::ImageFileManager::saveImage(&histImgCum,"histCum.pgm");

	//fcv::filterMedian(&img3,&img3,3);
	fcv::applyKernel(&img3,&img3,kernel,16);
	fcv::equalizeHistogram(&img3,&img3);
	fcv::ImageFileManager::saveImage(&img3,"equalized.pgm");

}


