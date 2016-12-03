/*
 * testMain.cpp
 *
 *  Created on: 09.10.2015
 *      Author: andreas
 */

#include "FreeCV/FreeCV.h"
#include <iostream>
#include <unistd.h>
#include <limits.h>

#define LOG_TEST_FKT_START(testName) LOG_INFO("Unit test ---------------- \033[32m" testName "\033[39m ----------------"); std::string unitName=testName;
#define LOG_TEST_FKT_END(valid){ LOG_FORMAT_INFO("Unit test ---------------- \033[32m%s \033[39m ----------------: %s", unitName.c_str(), valid?"\033[32m No Error!\033[39m ": "\033[31m Error! Test failed!\033[39m ");}
#define LOG_TEST_SUB_FKT_END(testName, valid) LOG_FORMAT_INFO("%s ... %s", testName, valid?"\033[32m OK\033[39m ": "\033[31m FAILED\033[39m ");
#define ERROR_IF_TEST_FAILED(res) if(!res){LOG_TEST_FKT_END(false);}

#define TEST_MAT_COMPONENTWISE(SIZE_X, SIZE_Y, TEST_FKT)\
	for (int y = 0; y < SIZE_Y; y++) {\
			for (int x = 0; x < SIZE_X; x++) {\
				if(!(TEST_FKT)){\
					valid = false;\
					break;\
				}\
			}\
			if(!valid)\
				break;\
		}\

// Forward declaration
bool TestMatrix();
bool TestMath();
bool TestSGM();
bool TestNeuro();

#ifdef WITH_CUDA
#include "FreeCV/Cuda/Cuda.h"
bool TestCuda();
#endif

int main(int argc, char **argv) {

	LOG_INFO("############################################################################\n"
			 "##################### Test functions for FreeCV-Library ####################\n"
			 "####################### developed by Andreas Rottach #######################\n"
			 "############################################################################\n");

	TestMatrix();
	TestMath();
//	TestSGM();
	TestNeuro();

#ifdef WITH_CUDA
	TestCuda();
#endif

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

//	fcv::Matrixf m(4,2),m2(2,3), m3;
//	m.setIdentity();
//	m.at(0,0) = 5;
//	m2.setIdentity();
//	std::cout <<m.toString()<< std::endl;
//	std::cout <<m2.toString()<< std::endl;

//	m3 = m*m2;
//	std::cout << m3.toString() << std::endl;


//	fcv::filterGauss(&img3,&img3,3);
////	fcv::applyKernel(&img3,&img3,kernel,16);
//	fcv::equalizeHistogram(&img3,&img3);
//	fcv::ImageFileManager::saveImage(&img3,"equalized.pgm");
//
//
//	fcv::Image img6 = fcv::ImageFileManager::loadImage("linienUndKreise.pgm");
//	fcv::imageHoughTransform(&img6,&img3);
//	fcv::ImageFileManager::saveImage(&img3,"hough.pgm",fcv::ImageFileManager::PGM_ASCII);

//	fcv::Image imgL = fcv::ImageFileManager::loadImage("stereo_left_gray.pgm");
//	fcv::Image imgLColor = fcv::ImageFileManager::loadImage("stereo_left_rgb.ppm");
//	fcv::Image imgR = fcv::ImageFileManager::loadImage("stereo_right_gray.pgm");
//
//	assert(imgL.isValid());
//	assert(imgLColor.isValid());
//	assert(imgR.isValid());
//
//	fcv::SGM sgm;
//	fcv::PointCloudCreator pcc;
//
//	sgm.init(imgL.getWidth(), imgL.getHeight(), 60);
//	sgm.updatePenalties(25, 250);
//	long memUsage = sgm.calcExpectedMemoryUsage();
//
//	std::cout << "Expected Memory Usage: " << memUsage / (1024*1024) << " MegaByte" << std::endl;
//
//	fcv::Vector2f c;
//	c[0] = imgL.getWidth()/2;
//	c[1] = imgL.getHeight()/2;
//	pcc.init(c,5299,0.117);
//
//	fcv::Image imgDispL(imgL.getWidth(), imgL.getHeight(), fcv::Image::PF_FLOAT_32);
//	fcv::Image imgDispR(imgL.getWidth(), imgL.getHeight(), fcv::Image::PF_FLOAT_32);
//
//	{
//		fcv::StopWatch sw("SGM processing");
//		sgm.processImagePair(&imgL, &imgR);
//		memcpy(imgDispL.getPtr<float>(), sgm.getDisparityData(), imgL.getWidth() * imgL.getHeight() * sizeof(float));
//
//		sgm.processImagePair(&imgL, &imgR, false);
//		memcpy(imgDispR.getPtr<float>(), sgm.getDisparityData(), imgL.getWidth() * imgL.getHeight() * sizeof(float));
//		{
//			fcv::StopWatch sw("L2R Consistency Check");
//			fcv::SGM::l2rConsistencyCheck(&imgDispL, &imgDispR, 1);
//		}
//	}
//	fcv::PointCloudCreator::PointCloud points;
//	pcc.convertDisparity(&imgDispL, &imgLColor, &points, 10);
//	fcv::PointCloudCreator::saveToPly("data.ply", &points);
//
//	fcv::Image imgDispColor;
//	fcv::convertToPseudoColor(&imgDispL, &imgDispColor, 0 , 60, 0, 120);
//
//	fcv::ImageFileManager::saveImage(&imgDispColor, "disparityL.ppm",
//			fcv::ImageFileManager::PPM_BINARY);
//
//
//	fcv::convertToPseudoColor(&imgDispR, &imgDispColor, 0 , 60, 0, 120);
//
//	fcv::ImageFileManager::saveImage(&imgDispColor, "disparityR.ppm",
//			fcv::ImageFileManager::PPM_BINARY);


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

bool TestMatrix()
{
	LOG_TEST_FKT_START("Matrix");

	// TODO Add more tests
	fcv::Matrix4x4f m1;
	m1.setIdentity();
	bool valid = true;
	TEST_MAT_COMPONENTWISE(4,4,m1.at(y,x) == (y == x));
	if(!valid){
		LOG_TEST_SUB_FKT_END("Identity Matrix test", false);
		LOG_TEST_FKT_END(false);
		return false;
	}else{
		LOG_TEST_SUB_FKT_END("Identity Matrix test", true);
	}

	fcv::Vector3f axis(0.325271,0.354611,0.448501);
	m1.rotate(axis, FCV_DEG2RAD(82.2648));
	fcv::Matrix4x4f correctRotMat (
			0.346197, -0.444925, 0.825948,0,
			0.906303, 0.386092, -0.171896,0,
			-0.242411, 0.808068, 0.536901,0,
			0,0,0,1);

//	LOG_FORMAT_INFO("%s", m1.toString().c_str());

	valid = true;
	TEST_MAT_COMPONENTWISE(4,4,fabs(m1.at(y,x)-correctRotMat.at(y,x)) <= 0.0001);
	if (!valid) {
		LOG_TEST_SUB_FKT_END("Rotation Matrix test", false);
		LOG_TEST_FKT_END(false);
		return false;
	} else {
		LOG_TEST_SUB_FKT_END("Rotation Matrix test", true);
	}



	LOG_TEST_FKT_END(true);
	return true;
}
bool TestMath()
{
	LOG_TEST_FKT_START("Math");

	fcv::Matrix3x3f m33(0),m33Tmp;
	fcv::Matrix3x3f L,U,LU,P;

	m33.at(0,0) = -1;
	m33.at(0,1) = 1;
	m33.at(0,2) = 1;
	m33.at(1,0) = 1;
	m33.at(1,1) = -3;
	m33.at(1,2) = -2;
	m33.at(2,0) = 5;
	m33.at(2,1) = 1;
	m33.at(2,2) = 4;

	fcv::Vector3f b,x;
	b[0] = 0;
	b[1] = 5;
	b[2] = 3;

	fcv::decomposeLU(m33,LU,P);
	// Extract L and U in single matrices
	for(int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			if(i==j){
				L.at(i,j) = 1;
				U.at(i,j) = LU.at(i,j);
			}
			else if(j>i) {
				L.at(i,j) = 0;
				U.at(i,j) = LU.at(i,j);
			}
			else
			{
				L.at(i,j) = LU.at(i,j);
				U.at(i,j) = 0;
			}
		}
	}
//	LOG_FORMAT_INFO("L: %s",L.toString().c_str());
//	LOG_FORMAT_INFO("U: %s",U.toString().c_str());
	// A = P⁻1*L*U;
	m33Tmp = P.transpose()*L*U;
//	LOG_FORMAT_INFO("%s",m44Tmp.toString().c_str());

	bool valid = true;
	TEST_MAT_COMPONENTWISE(3,3,fabs(m33.at(y,x)-m33Tmp.at(y,x)) <= 0.0001);
	if (!valid) {
		LOG_TEST_SUB_FKT_END("DecomposeLU test", false);
		LOG_TEST_FKT_END(false);
		return false;
	} else {
		LOG_TEST_SUB_FKT_END("DecomposeLU test", true);
	}

	fcv::solveLU(m33,b,x);

	if (fabs(x[0] - (-1))> 0.0001 || fabs(x[1] - (-4))> 0.0001 || fabs(x[2] - 3)> 0.0001) {
		LOG_TEST_SUB_FKT_END("Solve Ax=b test", false);
		LOG_TEST_FKT_END(false);
		return false;
	} else {
		LOG_TEST_SUB_FKT_END("Solve Ax=b test", true);
	}


	fcv::Matrix3x3f m33Inv;
	m33Inv = m33.inverse();
	m33Tmp = m33*m33Inv;

	TEST_MAT_COMPONENTWISE(3,3,fabs(m33Tmp.at(y,x)-(int)(x==y)) <= 0.0001);
	if (!valid) {
		LOG_TEST_SUB_FKT_END("Invert matrix test", false);
		LOG_TEST_FKT_END(false);
		return false;
	} else {
		LOG_TEST_SUB_FKT_END("Invert matrix test", true);
	}

	m33.at(0,0) = 25;
	m33.at(0,1) = 15;
	m33.at(0,2) = -5;
	m33.at(1,0) = 15;
	m33.at(1,1) = 18;
	m33.at(1,2) = 0;
	m33.at(2,0) = -5;
	m33.at(2,1) = 0;
	m33.at(2,2) = 11;
	fcv::decomposeCholesky(m33,L);
	fcv::Matrix3x3f L_t = L.transpose();
	m33Tmp = L*L_t;

	valid = true;
	TEST_MAT_COMPONENTWISE(3,3,fabs(m33.at(y,x)-m33Tmp.at(y,x)) <= 0.0001);
	if (!valid) {
		LOG_TEST_SUB_FKT_END("DecomposeCholesky test", false);
		LOG_TEST_FKT_END(false);
		return false;
	} else {
		LOG_TEST_SUB_FKT_END("DecomposeCholesky test", true);
	}


	LOG_TEST_FKT_END(true);
	return true;
}
bool TestSGM(){
	LOG_TEST_FKT_START("Semi global matching (SGM)");

	fcv::Image imgL = fcv::ImageFileManager::loadImage("stereo_left_gray.pgm");
	fcv::Image imgR = fcv::ImageFileManager::loadImage("stereo_right_gray.pgm");

	if(imgL.isValid() && imgR.isValid()){
		LOG_TEST_SUB_FKT_END("Loading Stereo images", true);
	}
	else{
		LOG_TEST_SUB_FKT_END("Loading Stereo images", false);
		LOG_TEST_FKT_END(false);
		return false;
	}

	fcv::SGM sgm;
	sgm.init(imgL.getWidth(), imgL.getHeight(), 60);
	sgm.updatePenalties(50, 250);
	long memUsage = sgm.calcExpectedMemoryUsage();
	LOG_FORMAT_INFO("Expected Memory Usage: %f MegaByte",memUsage / (1024.0 * 1024));

	fcv::Image imgDispL,imgDispR;
	{
		fcv::StopWatch sw("SGM processing L2R");
		sgm.processImagePair(&imgL, &imgR);
		imgDispL.init(imgL.getWidth(), imgL.getHeight(),
				fcv::Image::PF_FLOAT_32,
				(unsigned char*) sgm.getDisparityData(), true);

		fcv::ImageFileManager::saveImage(&imgDispL, "disparityLRaw.ppm",
				fcv::ImageFileManager::PGM_BINARY);
	}
	{
		fcv::StopWatch sw("SGM processing R2L");
		sgm.processImagePair(&imgL, &imgR, false);
		imgDispR.init(imgR.getWidth(), imgR.getHeight(),
				fcv::Image::PF_FLOAT_32,
				(unsigned char*) sgm.getDisparityData(), true);
		fcv::ImageFileManager::saveImage(&imgDispR, "disparityRRaw.ppm",
				fcv::ImageFileManager::PGM_BINARY);
	}
	{
		fcv::StopWatch sw("SGM L2R Consistency Check");
		fcv::SGM::l2rConsistencyCheck(&imgDispL, &imgDispR, 2);

		fcv::ImageFileManager::saveImage(&imgDispL, "disparityRRes.ppm",
				fcv::ImageFileManager::PGM_BINARY);
	}

	fcv::Image imgDispColor;
	fcv::convertToPseudoColor(&imgDispL, &imgDispColor, 0, 60, 0, 120);

	fcv::ImageFileManager::saveImage(&imgDispColor, "disparityL.ppm",
			fcv::ImageFileManager::PPM_BINARY);

	fcv::convertToPseudoColor(&imgDispR, &imgDispColor, 0, 60, 0, 120);

	fcv::ImageFileManager::saveImage(&imgDispColor, "disparityR.ppm",
			fcv::ImageFileManager::PPM_BINARY);

	LOG_TEST_FKT_END(true);
	return true;
}
bool TestNeuro(){
	LOG_TEST_FKT_START("Neuronal Network");

	fcv::Matrixd T(4,1);
	fcv::Matrixd X(4,2);
	// XOR
	//  X   | T
	// ----------
	// 0 0  | 0
	// 0 1  | 1
	// 1 0  | 1
	// 1 1  | 0

	T.at(0,0) = 0;
	T.at(1,0) = 1;
	T.at(2,0) = 1;
	T.at(3,0) = 0;

	X.at(0,0) = 0;
	X.at(0,1) = 0;
	X.at(1,0) = 0;
	X.at(1,1) = 1;
	X.at(2,0) = 1;
	X.at(2,1) = 0;
	X.at(3,0) = 1;
	X.at(3,1) = 1;

	fcv::MLPRProp mlp;
	fcv::MLPTransferTanh trans;
	mlp.initMLP(0.5,0.05,1.2,0.8,0.5,10e-6,10e-5,5000,2,5,1,(fcv::IMLPTransfer*)&trans);
	mlp.trainMLP(X,T);
	fcv::Vectord In(2),y2;
	In[0] = 1;
	In[0] = 0;
	mlp.evalMLP(In,&y2);

	LOG_TEST_SUB_FKT_END("Neuronal Network (RPROP)", true);


	LOG_TEST_FKT_END(true);
	return true;
}

#ifdef WITH_CUDA
bool TestCuda()
{
	LOG_TEST_FKT_START("Cuda GPU-Support");
	fcv::GPUData gpuData;
	int arr[100];
	int arr2[100];
	for(int i = 0; i < 100; i++)
	{
		arr[i] = i;
	}

	gpuData.uploadData(sizeof(int)*100,&arr);
	gpuData.downloadData(&arr2);
	bool valid = true;
	for(int i = 0; i < 100; i++)
	{
		if(arr2[i] != arr[i]){
			valid = false;
			break;
		}
	}

	if(valid){
		LOG_TEST_SUB_FKT_END("Upload and download data", true);
	}
	else{
		LOG_TEST_SUB_FKT_END("Upload and download data", false);
		LOG_TEST_FKT_END(false);
		return false;
	}

	LOG_TEST_FKT_END(true);
}
#endif
