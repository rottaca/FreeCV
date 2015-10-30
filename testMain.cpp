/*
 * testMain.cpp
 *
 *  Created on: 09.10.2015
 *      Author: andreas
 */

#include "FreeCV.h"

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
	fcv::Image imgR = fcv::ImageFileManager::loadImage("stereo_right_gray.pgm");

	assert(imgL.isValid());
	assert(imgR.isValid());

	fcv::SGM sgm;

	std::vector<int> penalties1;
//	penalties1.push_back(5);
	penalties1.push_back(10);
//	penalties1.push_back(20);

	std::vector<int> penalties2;
//	penalties2.push_back(60);
//	penalties2.push_back(80);
	penalties2.push_back(100);

	fcv::Image imgDisp(imgL.getWidth(), imgL.getHeight(),
			fcv::Image::PF_FLOAT_32);

	for (int i1 = 0; i1 < penalties1.size(); i1++) {
		for (int i2 = 0; i2 < penalties2.size(); i2++) {
			sgm.updatePenalties(penalties1.at(i1),penalties2.at(i2));
			sgm.init(imgL.getWidth(), imgL.getHeight(), 60);
			sgm.processImagePair(&imgL, &imgR);

//			fcv::Image imgCost(60, imgL.getWidth(), fcv::Image::PF_GRAYSCALE_8);

//			unsigned int* costData = sgm.getCostData();
//			unsigned char* imgData = imgCost.getPtr<unsigned char>();
//
//			for (int x = 0; x < imgL.getWidth(); x++) {
//				for (int d = 0; d < 60; d++) {
//					*imgData++ = *costData++;
//				}
//			}
//			fcv::ImageFileManager::saveImage(&imgCost, "cost.pgm",
//					fcv::ImageFileManager::PGM_BINARY);

			memcpy(imgDisp.getPtr<float*>(), sgm.getDisparityData(),
					imgL.getWidth() * imgL.getHeight() * sizeof(float));

			std::ostringstream str;
			str << "disp_" << penalties1.at(i1) << "_" << penalties2.at(i2) << ".pgm";

			fcv::ImageFileManager::saveImage(&imgDisp, str.str(),
					fcv::ImageFileManager::PGM_BINARY);
		}
	}


}

