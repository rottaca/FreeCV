/*
 * SGM.cpp
 *
 *  Created on: 28.10.2015
 *      Author: andreas
 */

#include "Stereo/SGM.h"
#include "assert.h"
#include "stdlib.h"
#include <limits>
#include <algorithm>

namespace fcv {

SGM::SGM() {
	m_isInitialized = false;
	m_CostData = NULL;

	// Define all evaluation paths
	m_paths.push_back(Path(1,0));
	m_paths.push_back(Path(-1,0));
	m_paths.push_back(Path(0,1));
	m_paths.push_back(Path(0,-1));

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

	m_CostData = new unsigned int[width*height*maxDisp];
	memset(m_CostData,0,width*height*maxDisp);

	m_aggregatedCosts = new unsigned int[width*height*maxDisp];
	memset(m_aggregatedCosts,0,width*height*maxDisp*sizeof(unsigned int));

	m_aggregatedCostsDir = new unsigned int[width*height*maxDisp];
	memset(m_aggregatedCostsDir,0,width*height*maxDisp*sizeof(unsigned int));

	m_disparityMap = new float[width*height];
	memset(m_disparityMap,0,width*height*sizeof(float));

	m_isInitialized = true;
	return true;
}
void SGM::calculateCost(Image* imgLeft, Image* imgRight)
{
	unsigned int* costPtr = m_CostData;
	unsigned char* imgLeftPtr = imgLeft->getPtr<unsigned char>();
	unsigned char* imgRightPtr = imgRight->getPtr<unsigned char>();
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			for (int d = 0; d < m_maxDisp; d++) {

				if (x - d < 0)
					*costPtr++ = 255;		// Sehr hohe kosten TODO: Richtig ?
				else {
					// TODO imgRightPtr nicht immer neu berechnen
					*costPtr++ = abs((int) (*imgLeftPtr) - *(imgRightPtr - d));	// = |L(y,x) - R(y,x-d)|
				}
			}
			imgLeftPtr++;
			imgRightPtr++;
		}
	}
}
void SGM::aggregateCost(){

	unsigned int* costPtr;
	unsigned int* aggrcostPtr;
	unsigned int* aggrcostPtrDirPrev;
	unsigned int* aggrcostPtrDir;

	int idxTopLeft = 0,
	idxTopRight = (m_width-1) * m_maxDisp,
	idxBottomLeft = (m_height-1)*m_width*m_maxDisp,
	idxBottomRight = ((m_height-1)*m_width + m_width-1)*m_maxDisp,
	addOneRow = m_maxDisp,
	addOneColumn = m_width*m_maxDisp;

	// Reset memory of aggreated costs
	memset(m_aggregatedCosts,0,m_height*m_width*m_maxDisp*sizeof(unsigned int));

	int xCurr,yCurr, steps;
	// Create an aggregated costs buffer for each path
	for(int p = 0; p < m_paths.size();p++){
		Path path = m_paths.at(p);


		// Check which side of the image is the start and init the whole image border on that side
		initAggregateCostDir(path);

		// from right to left
		if(path.x < 0)
		{
			// Accumulate costs
			// Go from right to left on each line
			aggrcostPtrDir = m_aggregatedCostsDir + idxBottomRight - addOneRow;
			costPtr = m_CostData  + idxBottomRight - addOneRow;

			for(int y = m_height-1; y >= 0 ; y--){
				for (int x = m_width - 2; x >= 0; x--) {
					evaluatePath(aggrcostPtrDir+addOneRow, costPtr, aggrcostPtrDir);
					aggrcostPtrDir-=addOneRow;
					costPtr-=addOneRow;
				}
				aggrcostPtrDir-=addOneRow;
				costPtr-=addOneRow;
			}
		}
		// From left to right
		else if (path.x > 0) {
			// Accumulate costs
			// Go from right to left on each line
			aggrcostPtrDir = m_aggregatedCostsDir + addOneRow;
			costPtr = m_CostData + addOneRow;

			for (int y = 0; y < m_height; y++) {
				for (int x = 1; x < m_width; x++) {
					evaluatePath(aggrcostPtrDir - addOneRow, costPtr,
							aggrcostPtrDir);
					aggrcostPtrDir += addOneRow;
					costPtr += addOneRow;
				}
				aggrcostPtrDir += addOneRow;
				costPtr += addOneRow;
			}
		}
		// from right to left
		else if(path.y < 0)
		{
			// Accumulate costs
			// Go from right to left on each line
			aggrcostPtrDir = m_aggregatedCostsDir + idxBottomRight - addOneColumn;
			costPtr = m_CostData + idxBottomRight - addOneColumn;

			for(int y = m_height-2; y >= 0 ; y--){
				for (int x = m_width - 1; x >= 0; x--) {
					evaluatePath(aggrcostPtrDir+addOneColumn, costPtr, aggrcostPtrDir);
					aggrcostPtrDir-=addOneRow;
					costPtr-=addOneRow;
				}
			}
		}
		// From left to right
		else if (path.y > 0) {

			// Accumulate costs
			// Go from right to left on each line
			aggrcostPtrDir = m_aggregatedCostsDir + idxTopLeft + addOneColumn;
			costPtr = m_CostData + idxTopLeft + addOneColumn;

			for(int y = m_height-2; y >= 0 ; y--){
				for (int x = m_width - 1; x >= 0; x--) {
					evaluatePath(aggrcostPtrDir-addOneColumn, costPtr, aggrcostPtrDir);
					aggrcostPtrDir+=addOneRow;
					costPtr+=addOneRow;
				}
			}
		}

		aggrcostPtr = m_aggregatedCosts;
		aggrcostPtrDir = m_aggregatedCostsDir;
		for(int i = 0; i < m_height*m_width*m_maxDisp; i++){
			*aggrcostPtr++ += *aggrcostPtrDir++;
		}

	}
}
void SGM::initAggregateCostDir(Path p)
{
	unsigned int* costPtr;
	unsigned int* aggrcostPtrDir;

	int idxTopLeft = 0,
	idxTopRight = (m_width-1) * m_maxDisp,
	idxBottomLeft = (m_height-1)*m_width*m_maxDisp,
	idxBottomRight = ((m_height-1)*m_width + m_width-1)*m_maxDisp,
	addOneRow = m_maxDisp,
	addOneColumn = m_width*m_maxDisp;

	memset(m_aggregatedCostsDir,0,m_height*m_width*m_maxDisp*sizeof(unsigned int));

	// from right to left
	if (p.x < 0) {
		// Point to the last column
		costPtr = m_CostData + idxTopRight;
		aggrcostPtrDir = m_aggregatedCostsDir + idxTopRight;
		// Copy Data
		for (int h = 0; h < m_height; h++) {
			memcpy(aggrcostPtrDir, costPtr, sizeof(unsigned int) * m_maxDisp);// Copy all DisparityCosts of a pixel
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
			memcpy(aggrcostPtrDir, costPtr, sizeof(unsigned int) * m_maxDisp);// Copy all DisparitesCosts of a pixel
			costPtr += addOneColumn;
			aggrcostPtrDir += addOneColumn;
		}
	}
	else if (p.y < 0) {
		// Point to the last row
		costPtr = m_CostData + idxBottomLeft;
		aggrcostPtrDir = m_aggregatedCostsDir + idxBottomLeft;
		// Copy Data
		memcpy(aggrcostPtrDir, costPtr,
				sizeof(unsigned int) * m_maxDisp * m_width);// Copy all DisparitesCosts of a pixel
	}
	// From left to right
	else if (p.y > 0) {
		// Point to the first row
		costPtr = m_CostData;
		aggrcostPtrDir = m_aggregatedCostsDir;
		// Copy Data
		memcpy(aggrcostPtrDir, costPtr,
				sizeof(unsigned int) * m_maxDisp * m_width);// Copy all DisparitesCosts of a pixel
	}

}
void SGM::evaluatePath(unsigned int* priorAccPtr, unsigned int* currCostPtr, unsigned int* currentAccPtr)
{
	// Add current cost
	memcpy(currentAccPtr,currCostPtr,m_maxDisp*sizeof(unsigned int));

	// Calculate minimum of previous accumulation
	for(int d = 0;  d < m_maxDisp; d++){
		unsigned int minPrevCost = std::numeric_limits<unsigned int>::max();

		for(int d_prev = 0;  d_prev < m_maxDisp; d_prev++){
			// No penalty
			if(d == d_prev){
				minPrevCost = std::min(minPrevCost,priorAccPtr[d_prev]);
			}
			// Small penalty
			else if( abs(d-d_prev) == 1){
				minPrevCost = std::min(minPrevCost, priorAccPtr[d_prev] + m_penalty1);
			}
			// Large penalty
			else{
				minPrevCost = std::min(minPrevCost, priorAccPtr[d_prev] + m_penalty2);
			}
		}
		// Add prior cost
		*currentAccPtr += minPrevCost;
		// Normalize
		*currentAccPtr -= *std::min_element(priorAccPtr,priorAccPtr + m_maxDisp);
		// Calculate next
		currentAccPtr++;
	}
}
void SGM::computeDisparityMap()
{
	float* ptr = m_disparityMap;
	unsigned int* aggCostPtr = m_aggregatedCosts;
	unsigned int* costPtr = m_CostData;

	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
//			if (x > m_maxDisp) {
				// Index is Disparity
				int minIdx = std::min_element(aggCostPtr,
						aggCostPtr + m_maxDisp) - aggCostPtr;
				*ptr++ = minIdx;
//			} else
//				*ptr++ = 0;

			costPtr += m_maxDisp;
			aggCostPtr += m_maxDisp;
		}
	}
}
bool SGM::processImagePair(Image* imgLeft, Image* imgRight)
{
	assert(imgLeft->getFormat() == Image::PF_GRAYSCALE_8 && imgRight->getFormat() == Image::PF_GRAYSCALE_8);
	assert(imgLeft->getWidth() == imgRight->getWidth() && imgLeft->getHeight() == imgRight->getHeight());
	assert(imgLeft->getWidth() == m_width && imgLeft->getHeight() == m_height);

	// Cost calculation
	calculateCost(imgLeft,imgRight);
	// Aggreate calculation
	aggregateCost();
	// Calculate Disparity Map
	computeDisparityMap();

	return true;
}

} /* namespace fcv */
