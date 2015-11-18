/*
 * PointCloudCreator.cpp
 *
 *  Created on: 03.11.2015
 *      Author: andreas
 */

#include <FreeCV/Stereo/PointCloudCreator.h>
#include "assert.h"
#include "FreeCV/Core/Logger.h"

namespace fcv {

PointCloudCreator::PointCloudCreator() {
	// TODO Auto-generated constructor stub

}

PointCloudCreator::~PointCloudCreator() {
	// TODO Auto-generated destructor stub
}


bool PointCloudCreator::init(Vector2f C, float f, float baseLine)
{
	m_c = C;
	m_f = f;
	m_BaseLine = baseLine;
	return true;
}

bool PointCloudCreator::saveToPly(std::string fileName, PointCloud* points)
{
	std::ofstream f(fileName.c_str(),
			std::ios_base::out | std::ios_base::trunc);

	f << "ply"<< std::endl;
	f << "format ascii 1.0" << std::endl;
	f << "element vertex " << points->size() << std::endl;
	f << "property float x" << std::endl;
	f << "property float y" << std::endl;
	f << "property float z" << std::endl;
	f << "property uchar red" << std::endl;
	f << "property uchar green" << std::endl;
	f << "property uchar blue" << std::endl;
	f << "element face 0" << std::endl;
	f << "end_header" << std::endl;

	for(int i = 0; i < points->size(); i++){
		PixelData pd = points->at(i);

		f << pd.pos[0] << " " << pd.pos[1] << " " << pd.pos[2];

		f << " " << (int) pd.color[0];
		f << " " << (int) pd.color[1];
		f << " " << (int) pd.color[2];

		f << std::endl;

	}
	f.close();
	return true;
}

bool PointCloudCreator::loadFromPly(std::string fileName, PointCloud* points)
{
	std::ifstream f(fileName.c_str(),
			std::ios_base::in);

	points->clear();
	std::string str;

	std::getline(f,str);
	if(str.find("ply") == std::string::npos){
		LOG_ERROR("Invalid ply file!");
		return false;
	}

	std::getline(f,str);
	if(str.find("format ascii 1.0") == std::string::npos){
		LOG_ERROR("Only format ascii 1.0 supported!");
		return false;
	}

	std::getline(f,str);
	if(str.find("element vertex") == std::string::npos){
		LOG_ERROR("Invalid text!");
		return false;
	}
	std::vector<std::string> props;
	while(std::getline(f,str) && str.find("end_header") == std::string::npos){
		if(str.find("property") != std::string::npos)
			props.push_back(str);
	}
	while(std::getline(f,str)){
		std::stringstream lineStream(str);
		int propIdx = 0;
		PixelData p;
		for(;propIdx < props.size(); propIdx++){
			std::string prop = props.at(propIdx);
			if(prop.find("float x") != std::string::npos){
				lineStream >> p.pos[0];
			}
			else if(prop.find("float y") != std::string::npos){
				lineStream >> p.pos[1];
			}
			else if(prop.find("float z") != std::string::npos){
				lineStream >> p.pos[2];
			}
			else if(prop.find("uchar red") != std::string::npos){
				lineStream >> p.color[0];
			}
			else if(prop.find("uchar green") != std::string::npos){
				lineStream >> p.color[1];
			}
			else if(prop.find("uchar blue") != std::string::npos){
				lineStream >> p.color[2];
			}else{
				LOG_ERROR("Unknown property!");
			}
		}
		points->push_back(p);
	}
	return true;
}
bool PointCloudCreator::convertDisparity(Image* dispImg, Image* imageRaw, PointCloud* points, float minDisp, float maxDisp)
{
	assert(dispImg->getFormat() == Image::PF_FLOAT_32);

	float* dispData = dispImg->getPtr<float>();
	unsigned char* imgData;

	if(imageRaw != NULL)
		imgData = imageRaw->getPtr<unsigned char>();

	for (int y = 0; y < dispImg->getHeight(); y++) {
		for (int x = 0; x < dispImg->getWidth(); x++) {
			PixelData pd;
			float disp = *dispData++;
			bool use = (minDisp == -1 || minDisp <= disp) && (maxDisp == -1 || maxDisp >= disp) && disp > 0;

			if (use) {
				float depth = m_f * m_BaseLine / disp;
				Vector4f pos;
				pos[0] = (x - m_c[0]) / m_f;
				pos[1] = (y - m_c[1]) / m_f;
				pos[2] = 1;
				pos = pos * depth;
				pos[3] = 1;

				if(!m_H_world_cam.isEmpty())
				{
					pos = m_H_world_cam*pos;
				}

				pd.pos[0] = pos[0];
				pd.pos[1] = pos[1];
				pd.pos[2] = pos[2];

				if (imageRaw != NULL) {
					if (imageRaw->getFormat() == Image::PF_GRAYSCALE_8) {
						pd.color[0] = *imgData;
						pd.color[1] = *imgData;
						pd.color[2] = *imgData;
						imgData++;
					} else if (imageRaw->getFormat() == Image::PF_RGB_888) {
						pd.color[0] = *imgData++;
						pd.color[1] = *imgData++;
						pd.color[2] = *imgData++;
					}
				}

				points->push_back(pd);
			}else{
				if (imageRaw->getFormat() == Image::PF_GRAYSCALE_8) {
					imgData++;
				} else if (imageRaw->getFormat() == Image::PF_RGB_888) {
					imgData+= 3;
				}
			}
		}
	}
	return true;
}
} /* namespace fcv */
