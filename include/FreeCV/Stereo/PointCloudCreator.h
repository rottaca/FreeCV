/*
 * PointCloudCreator.h
 *
 *  Created on: 03.11.2015
 *      Author: andreas
 */

#ifndef POINTCLOUDCREATOR_H_
#define POINTCLOUDCREATOR_H_

#include <FreeCV/Core/Vector.h>
#include <string>
#include <vector>
#include <fstream>

#include <FreeCV/Image/Image.h>
#include <FreeCV/Core/Matrix.h>

namespace fcv {

class PointCloudCreator {
public:
	typedef struct{
		Vector3f pos;
		Vector3f color;
	} PixelData;

	typedef std::vector<PixelData> PointCloud;

	PointCloudCreator();
	virtual ~PointCloudCreator();

	bool init(Vector2f C, float f, float baseLine);
	void setCameraPose(Matrix4x4f pose){
		m_H_world_cam = pose;
	}

	bool convertDisparity(Image* dispImg, Image* imageRaw, PointCloud* points, float minDisp = -1, float maxDisp = -1);

	static bool saveToPly(std::string fileName, PointCloud* points);
	static bool loadFromPly(std::string fileName, PointCloud* points);

private:
    Vector2f m_c;
    float m_f,m_BaseLine;
    Matrix4x4f m_H_world_cam;
};

} /* namespace fcv */

#endif /* POINTCLOUDCREATOR_H_ */
