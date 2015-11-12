/*
 * SGM.cpp
 *
 *  Created on: 28.10.2015
 *      Author: andreas
 */

#include "FreeCV/Stereo/SGM.h"
#include "FreeCV/Core/StopWatch.h"
#include "assert.h"
#include "stdlib.h"
#include <limits>
#include <algorithm>
#include <iostream>
#include <math.h>

using namespace std;

namespace fcv {

SGM::SGM() {
	m_isInitialized = false;
	m_CostData = NULL;

	// Define all evaluation paths
	m_paths.push_back(Path(1,0));
	m_paths.push_back(Path(-1,0));
	m_paths.push_back(Path(0,1));
	m_paths.push_back(Path(0,-1));
	m_paths.push_back(Path(1,1));
	m_paths.push_back(Path(-1,-1));
	m_paths.push_back(Path(1,-1));
	m_paths.push_back(Path(-1,1));

	updatePenalties(10,100);
}

SGM::~SGM() {
	deinit();
}


void SGM::deinit(){
	if(m_isInitialized)
	{
		delete[] m_CostData;
		m_CostData = NULL;
		delete[] m_aggregatedCostsDir;
		m_aggregatedCostsDir = NULL;
		delete[] m_disparityMap;
		m_disparityMap = NULL;
		delete[] m_aggregatedCosts;
		m_aggregatedCosts = NULL;
		m_isInitialized = false;
	}
}
bool SGM::init(int width, int height, int maxDisp){
	assert(width != 0);
	assert(height != 0);
	assert(maxDisp != 0);

	deinit();
	m_width = width;
	m_height = height;
	m_maxDisp = maxDisp;
	m_dataSize = m_width*m_height*m_maxDisp;
	m_buffSize = m_dataSize*sizeof(unsigned short);

	m_CostData = new unsigned short[width*height*maxDisp];
	memset(m_CostData,0,m_dataSize);

	m_aggregatedCosts = new unsigned short[width*height*maxDisp];
	memset(m_aggregatedCosts,0,m_buffSize);

	m_aggregatedCostsDir = new unsigned short[width*height*maxDisp];
	memset(m_aggregatedCostsDir,0,m_buffSize);

	m_disparityMap = new float[width*height];
	memset(m_disparityMap,0,width*height*sizeof(float));

	m_isInitialized = true;
	return true;
}
void SGM::calculateCost(Image* imgLeft, Image* imgRight)
{

#pragma omp parallel for
	for (int y = 0; y < m_height; y++) {
		unsigned short* costPtr = m_CostData + m_width*m_maxDisp*y;
		unsigned char* imgLeftPtr = imgLeft->getPtr<unsigned char>(y);
		unsigned char* imgRightPtr = imgRight->getPtr<unsigned char>(y);
		for (int x = 0; x < m_width; x++) {
			for (int d = 0; d < m_maxDisp; d++) {

				if(m_Left2Right){
					if (x - d < 0)
						*costPtr++ = 255;		// Sehr hohe kosten TODO: Richtig ?
					else {
						// TODO imgRightPtr nicht immer neu berechnen
						*costPtr++ = abs((*imgLeftPtr) - *(imgRightPtr - d));	// = |L(y,x) - R(y,x-d)|
					}
				}else{
					if (x + d >= m_width)
						*costPtr++ = 255;	// Sehr hohe kosten TODO: Richtig ?
					else {
						// TODO imgRightPtr nicht immer neu berechnen
						*costPtr++ = abs((*imgRightPtr) - *(imgLeftPtr + d));// = |L(y,x-d) - R(y,x)|
					}
				}
			}
			imgLeftPtr++;
			imgRightPtr++;
		}
	}
	// Scale to 11 bits
//	ununsigned shortax = *max_element(m_CostData,
//			m_CostData + m_dataSize);
//
//	costPtr = m_CostData;
//	for (int i = 0; i < m_dataSize; i++)
//		*costPtr++ = *costPtr * 2047 / max;

}
void SGM::aggregateCost(){


	int idxTopLeft = 0,
	idxTopRight = (m_width-1) * m_maxDisp,
	idxBottomLeft = (m_height-1)*m_width*m_maxDisp,
	idxBottomRight = ((m_height-1)*m_width + m_width-1)*m_maxDisp,
	addOneRow = m_maxDisp,
	addOneColumn = m_width*m_maxDisp,
	offs;

	// Reset memory of aggreated costs
	memset(m_aggregatedCosts,0, m_buffSize);

	int xCurr,yCurr;
	// Create an aggregated costs buffer for each path
	for(int p = 0; p < m_paths.size();p++){
		Path path = m_paths.at(p);
//#ifndef NDEBUG
		cout << "Aggregate Path: " << path.x << " " << path.y << endl;
//#endif
		// Check which side of the image is the start and init the whole image border on that side
		initAggregateCostDir(path);
		// Offset to next pixel
		offs = path.y*addOneColumn + path.x*addOneRow;

		// from right to left
		if(path.x < 0)
		{
			// Accumulate costs
			// Go from right to left on each line

//#ifndef NDEBUG
		cout << "Aggregate -X-Dir" << endl;
//#endif

			for(int y = 0; y  < m_height; y++){
				unsigned short* costPtr;
				unsigned short* aggrcostPtrDirPrev;
				unsigned short* aggrcostPtrDir;
				// Point to last row
				aggrcostPtrDir = m_aggregatedCostsDir + idxTopRight + y*addOneColumn;
				aggrcostPtrDirPrev = aggrcostPtrDir - offs;
				costPtr = m_CostData  + idxTopRight + y*addOneColumn;


//				cout << "loop: " << y << endl;
				// Pointer still in image?
				xCurr = m_width-1;
				yCurr = y;
				while(xCurr >= 0 && yCurr >= 0 && xCurr < m_width && yCurr < m_height){


//					cout << "x: "<< x << endl;
					evaluatePath(aggrcostPtrDirPrev, costPtr, aggrcostPtrDir);

					aggrcostPtrDirPrev = aggrcostPtrDir;
					aggrcostPtrDir+=offs;
					costPtr+=offs;
					xCurr += path.x;
					yCurr += path.y;
				}
			}
		}
		// From left to right
		else if (path.x > 0) {
			// Accumulate costs
			// Go from right to left on each line
//#ifndef NDEBUG
		cout << "Aggregate X-Dir" << endl;
//#endif

			for(int y = 0; y  < m_height ; y++){
				unsigned short* costPtr;
				unsigned short* aggrcostPtrDirPrev;
				unsigned short* aggrcostPtrDir;
				// Point to last row
				aggrcostPtrDir = m_aggregatedCostsDir + idxTopLeft + y*addOneColumn;
				aggrcostPtrDirPrev = aggrcostPtrDir - offs;
				costPtr = m_CostData  + idxTopLeft + y*addOneColumn;

				if(aggrcostPtrDirPrev < m_aggregatedCostsDir)
					continue;

				// Pointer still in image?
				xCurr = 0;
				yCurr = y;
				while(xCurr >= 0 && yCurr >= 0 && xCurr < m_width && yCurr < m_height){


//					cout << "x: "<< x << endl;
					evaluatePath(aggrcostPtrDirPrev, costPtr, aggrcostPtrDir);

					aggrcostPtrDirPrev = aggrcostPtrDir;
					aggrcostPtrDir+=offs;
					costPtr+=offs;
					xCurr += path.x;
					yCurr += path.y;
				}
			}
		}
		// from right to left
		if(path.y < 0)
		{
			// Accumulate costs
			// Go from right to left on each line
//#ifndef NDEBUG
		cout << "Aggregate -Y-Dir" << endl;
//#endif

			for(int x = 0; x  < m_width ; x++){
				unsigned short* costPtr;
				unsigned short* aggrcostPtrDirPrev;
				unsigned short* aggrcostPtrDir;
				// Point to last row
				aggrcostPtrDir = m_aggregatedCostsDir + idxBottomLeft + x*addOneRow;
				aggrcostPtrDirPrev = aggrcostPtrDir - offs;
				costPtr = m_CostData + idxBottomLeft  + x*addOneRow;

				// Pointer still in image?
				xCurr = x;
				yCurr = m_height-1;
				while(xCurr >= 0 && yCurr >= 0 && xCurr < m_width && yCurr < m_height){

					evaluatePath(aggrcostPtrDirPrev, costPtr, aggrcostPtrDir);

					aggrcostPtrDirPrev = aggrcostPtrDir;
					aggrcostPtrDir+=offs;
					costPtr+=offs;
					xCurr += path.x;
					yCurr += path.y;

				}
			}
		}
		// From left to right
		else if (path.y > 0) {

			// Accumulate costs
			// Go from right to left on each line
//#ifndef NDEBUG
		cout << "Aggregate Y-Dir" << endl;
//#endif

			for(int x = 0; x  < m_width ; x++){
				unsigned short* costPtr;
				unsigned short* aggrcostPtrDirPrev;
				unsigned short* aggrcostPtrDir;
				// Point to last row
				aggrcostPtrDir = m_aggregatedCostsDir + x*addOneRow + addOneColumn;
				aggrcostPtrDirPrev = aggrcostPtrDir - offs;
				costPtr = m_CostData + x*addOneRow + addOneColumn;

				if (aggrcostPtrDirPrev < m_aggregatedCostsDir)
					continue;

				// Pointer still in image?
				xCurr = x;
				yCurr = 1;
				while(xCurr >= 0 && yCurr >= 0 &&
						xCurr < m_width && yCurr < m_height){


					evaluatePath(aggrcostPtrDirPrev, costPtr, aggrcostPtrDir);

					aggrcostPtrDirPrev = aggrcostPtrDir;
					aggrcostPtrDir+=offs;
					costPtr+=offs;
					xCurr += path.x;
					yCurr += path.y;
				}
			}
		}

//		aggrcostPtr = m_aggregatedCosts;
//		aggrcostPtrDir = m_aggregatedCostsDir;

		for(int i = 0; i < m_dataSize; i++){
			m_aggregatedCosts[i] += m_aggregatedCostsDir[i];
		}

	}
}
void SGM::initAggregateCostDir(Path p)
{
	unsigned short* costPtr;
	unsigned short* aggrcostPtrDir;

	int idxTopLeft = 0,
	idxTopRight = (m_width-1) * m_maxDisp,
	idxBottomLeft = (m_height-1)*m_width*m_maxDisp,
	idxBottomRight = ((m_height-1)*m_width + m_width-1)*m_maxDisp,
	addOneRow = m_maxDisp,
	addOneColumn = m_width*m_maxDisp;

	memset(m_aggregatedCostsDir,0,m_buffSize);

	// from right to left
	if (p.x < 0) {
		// Point to the last column
		costPtr = m_CostData + idxTopRight;
		aggrcostPtrDir = m_aggregatedCostsDir + idxTopRight;
		// Copy Data
		for (int h = 0; h < m_height; h++) {
			memcpy(aggrcostPtrDir, costPtr, sizeof(unsigned short) * m_maxDisp);// Copy all DisparityCosts of a pixel
			costPtr += addOneColumn;
			aggrcostPtrDir += addOneColumn;
		}
	}
	// From left to right
	else if (p.x > 0) {

		// Point to the first column
		costPtr = m_CostData;
		aggrcostPtrDir = m_aggregatedCostsDir;
		// Copy Data
		for (int h = 0; h < m_height; h++) {
			memcpy(aggrcostPtrDir, costPtr, sizeof(unsigned short) * m_maxDisp);// Copy all DisparitesCosts of a pixel
			costPtr += addOneColumn;
			aggrcostPtrDir += addOneColumn;
		}
	}

	if (p.y < 0) {
		// Point to the last row
		costPtr = m_CostData + idxBottomLeft;
		aggrcostPtrDir = m_aggregatedCostsDir + idxBottomLeft;
		// Copy Data
		memcpy(aggrcostPtrDir, costPtr,
				sizeof(unsigned short) * m_maxDisp * m_width);// Copy all DisparitesCosts of a pixel
	}
	// From left to right
	else if (p.y > 0) {
		// Point to the first row
		costPtr = m_CostData;
		aggrcostPtrDir = m_aggregatedCostsDir;
		// Copy Data
		memcpy(aggrcostPtrDir, costPtr,
				sizeof(unsigned short) * m_maxDisp * m_width);// Copy all DisparitesCosts of a pixel
	}

}
inline void SGM::evaluatePath(unsigned short* priorAccPtr, unsigned short* currCostPtr, unsigned short* currentAccPtr)
{
	// Add current cost
	memcpy(currentAccPtr,currCostPtr,m_maxDisp*sizeof(unsigned short));
	// Can be precalculated
	unsigned short prevMin = *min_element(priorAccPtr, (unsigned short*)(priorAccPtr + m_maxDisp));

	// Calculate minimum of previous accumulation
	for(int d = 0;  d < m_maxDisp; d++){
		unsigned short minPrevCost = numeric_limits<unsigned short>::max();

		for(int d_prev = 0;  d_prev < m_maxDisp; d_prev++){
			// No penalty
			if(d == d_prev){
				minPrevCost = min(minPrevCost,priorAccPtr[d_prev]);
			}
			// Small penalty
			else if( abs(d-d_prev) == 1){
				minPrevCost = min(minPrevCost, (unsigned short)(priorAccPtr[d_prev] + m_penalty1));
			}
			// Large penalty
			else{
				minPrevCost = min(minPrevCost, (unsigned short)(priorAccPtr[d_prev] + m_penalty2));
			}
		}
		// Add prior cost
		currentAccPtr[d] += minPrevCost;
		// Normalize
		currentAccPtr[d] -= prevMin;
		// Calculate next
		//currentAccPtr++;
	}
}
void SGM::computeDisparityMap()
{

#pragma omp parallel for
	for (int i = 0; i < m_height * m_width; i++) {

		unsigned short* aggCostPtr = m_aggregatedCosts + m_maxDisp*i;
		unsigned short* costPtr = m_CostData + m_maxDisp*i;

		// Index is Disparity
		m_disparityMap[i] = min_element(aggCostPtr, aggCostPtr + m_maxDisp)	- aggCostPtr;

//		costPtr += m_maxDisp;
//		aggCostPtr += m_maxDisp;
	}
}
bool SGM::processImagePair(Image* imgLeft, Image* imgRight, bool Left2Right)
{
	m_Left2Right = Left2Right;
	assert(imgLeft->getFormat() == Image::PF_GRAYSCALE_8 && imgRight->getFormat() == Image::PF_GRAYSCALE_8);
	assert(imgLeft->getWidth() == imgRight->getWidth() && imgLeft->getHeight() == imgRight->getHeight());
	assert(imgLeft->getWidth() == m_width && imgLeft->getHeight() == m_height);

	cout << "Calculate Costs" << endl;
	// Cost calculation
	{
		fcv::StopWatch sw("calculateCost");
		calculateCost(imgLeft, imgRight);
	}
	cout << "Aggregate Costs" << endl;
	{
		fcv::StopWatch sw("aggregateCost");
		// Aggreate calculation
		aggregateCost();
	}
	cout << "Compute Disparity Map" << endl;
	{
		fcv::StopWatch sw("computeDisparityMap");
		// Calculate Disparity Map
		computeDisparityMap();
	}
	return true;
}

bool SGM::l2rConsistencyCheck(Image* dispLeft, Image* dispRight, int threshold)
{
	assert(dispLeft->getFormat() == Image::PF_FLOAT_32 && dispRight->getFormat() == Image::PF_FLOAT_32);
	assert(dispLeft->getWidth() == dispRight->getWidth() && dispLeft->getHeight() == dispRight->getHeight());

	float* dispLeftPtr = dispLeft->getPtr<float>(0);
	float* dispRightPtr = dispRight->getPtr<float>(0);
	for (int y = 0; y < dispRight->getHeight(); y++) {

		for (int x = 0; x < dispRight->getWidth(); x++) {

			float dispL = *dispLeftPtr;
			int offsRight = roundf(dispL);

			if(offsRight < 0)
			{
				*dispLeftPtr = -1;
				dispLeftPtr++;
				dispRightPtr++;
				continue;
			}

			if(x - offsRight < 0 || x - offsRight >= dispRight->getWidth())
			{
				*dispLeftPtr = -1;
			}
			else
			{
				float dispR = *(dispRightPtr - offsRight);

				if(abs((int)(dispL - dispR)) > threshold)
				{
					*dispLeftPtr = -1;
					float* p = dispRightPtr - offsRight;
					*p = -1;
				}
			}

			dispLeftPtr++;
			dispRightPtr++;
		}
	}

	dispLeftPtr = dispLeft->getPtr<float>();
	dispRightPtr = dispRight->getPtr<float>();
	for (int y = 0; y < dispRight->getHeight(); y++) {

		for (int x = 0; x < dispRight->getWidth(); x++) {

			float dispR = *dispRightPtr;
			int offsetLeft = roundf(dispR);

			if(offsetLeft < 0)
			{
				*dispRightPtr = -1;
				dispLeftPtr++;
				dispRightPtr++;
				continue;
			}

			if(x + offsetLeft < 0 || x + offsetLeft >= dispRight->getWidth())
			{
				*dispRightPtr = -1;
			}
			else
			{
				float dispL = *(dispLeftPtr + offsetLeft);

				if(abs((int)(dispL - dispR)) > threshold)
				{
					*dispRightPtr = -1;
					float* p = dispLeftPtr + offsetLeft ;
					*p = -1;
				}
			}

			dispLeftPtr++;
			dispRightPtr++;
		}
	}

	return true;
}

} /* namespace fcv */
