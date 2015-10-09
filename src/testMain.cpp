/*
 * testMain.cpp
 *
 *  Created on: 09.10.2015
 *      Author: andreas
 */

#include "Vector.h"
#include "Matrix.h"
#include "Image.h"
#include "ImageFileManager.h"
#include "Logger.h"

int main(int argc, char **argv) {
	fcv::Vector3d v1;
	v1[0] = 5;
	v1[1] = 4;
	v1[2] = 3;

	fcv::Vector3d v2;
	v2[0] = 1;
	v2[1] = 0;
	v2[2] = 0;

	LOG_FORMAT_INFO("v1 %s", v1.toString().c_str());
	LOG_FORMAT_INFO("v2 %s", v2.toString().c_str());
	fcv::Vector3d vTmp = v1+v2;
	LOG_FORMAT_INFO("v1+v2: %s", vTmp.toString().c_str());
	vTmp = v1*6;
	LOG_FORMAT_INFO("v1*6: %s", vTmp.toString().c_str());

//	vTmp = v1*v2;
//	LOG_FORMAT_INFO("v1*v2: %s", vTmp.toString().c_str());

	fcv::Matrix3x3d mat;
	mat.at(0,0) = 0;
	mat.at(0,1) = -1;
	mat.at(0,2) = 0;

	mat.at(1,0) = 1;
	mat.at(1,1) = 0;
	mat.at(1,2) = 0;

	mat.at(2,0) = 0;
	mat.at(2,1) = 0;
	mat.at(2,2) = 1;
	LOG_FORMAT_INFO("mat: %s", mat.toString().c_str());

	vTmp = mat*v2;
	LOG_FORMAT_INFO("mat*v2: %s", vTmp.toString().c_str());

	fcv::Image img(100,100,fcv::Image::RGB_888);
	fcv::Image img2(img,true);
	fcv::Image img3(250,250,fcv::Image::GRAYSCALE_8);

	LOG_FORMAT_INFO("Img 1: %s", img.toString().c_str());
	LOG_FORMAT_INFO("Img 2: %s", img2.toString().c_str());
	LOG_FORMAT_INFO("Img 3: %s", img3.toString().c_str());

	memset(img2.getPtr<unsigned char>(),128,img2.getWidth()*img2.getHeight()*3);
	fcv::ImageFileManager::saveImage(img2,"test.ppm");
	memset(img3.getPtr<unsigned char>(),255,img3.getWidth()*img3.getHeight());
	fcv::ImageFileManager::saveImage(img3,"test2.pgm");
	img3 = fcv::ImageFileManager::loadImage("test2.pgm");
	fcv::ImageFileManager::saveImage(img3,"test2.pgm");

}


