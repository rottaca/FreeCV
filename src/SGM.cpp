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

#ifdef WITH_OPENMP
#include <omp.h>
#endif

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
		delete[] m_tmpAggrCost;
		m_tmpAggrCost = NULL;
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

	m_tmpAggrCost = new unsigned short[3*width*maxDisp + maxDisp];
	memset(m_tmpAggrCost,0,(3*width*maxDisp + maxDisp)*sizeof(short));

	m_disparityMap = new float[width*height];
	memset(m_disparityMap,0,width*height*sizeof(float));
	m_isInitialized = true;
	return true;
}
void SGM::calculateCost(Image* imgLeft, Image* imgRight)
{
	if (imgLeft->getFormat() == Image::PF_GRAYSCALE_8) {
		unsigned short* costPtr = m_CostData;
		unsigned char* imgLeftPtr = imgLeft->getPtr<unsigned char>();
		unsigned char* imgRightPtr = imgRight->getPtr<unsigned char>();
		for (int y = 0; y < m_height; y++) {
			for (int x = 0; x < m_width; x++) {
				for (int d = 0; d < m_maxDisp; d++) {

					if (m_Left2Right) {
						if (x - d < 0)
							*costPtr++ = 255;// Sehr hohe kosten TODO: Richtig ?
						else {
							// TODO imgRightPtr nicht immer neu berechnen
							*costPtr++ = abs(
									(*imgLeftPtr) - *(imgRightPtr - d));// = |L(y,x) - R(y,x-d)|
						}
					} else {
						if (x + d >= m_width)
							*costPtr++ = 255;// Sehr hohe kosten TODO: Richtig ?
						else {
							// TODO imgRightPtr nicht immer neu berechnen
							*costPtr++ = abs(
									(*imgRightPtr) - *(imgLeftPtr + d));// = |L(y,x-d) - R(y,x)|
						}
					}
				}
				imgLeftPtr++;
				imgRightPtr++;
			}
		}
		// Matching cost census
		// Window size: 5x5 -> 25 bits
//		unsigned int censusL, censusR;
//		unsigned int distToStart = (m_width+2) * m_maxDisp * 2;
//		memset(m_CostData,0,m_width*m_height*m_maxDisp*sizeof(unsigned short));
//
//		unsigned short* costPtr = m_CostData +  m_width * m_maxDisp * 2 + 2*m_maxDisp;
//		unsigned char* imgLeftPtr = imgLeft->getPtr<unsigned char>(2,2);
//		unsigned char* imgRightPtr = imgRight->getPtr<unsigned char>(2,2);
//
//		for (int y = 2; y < m_height-2; y++) {
//			for (int x = 2; x < m_width-2; x++) {
//				for (int d = 0; d < m_maxDisp; d++) {
//					censusL = 0;
//					censusR = 0;
//					if (m_Left2Right) {

//						if (x - d < 0)
//							*costPtr++ = 255; // Sehr hohe kosten TODO: Richtig ?
//						else {
//
//							unsigned char pixVal = *imgLeftPtr;
//							unsigned char* ptrL = imgLeftPtr - distToStart;
//
//							int cnt = 0;
//							for (int i = 0; i < 5; i++) {
//								for (int j = 0; j < 5; j++) {
//									if (*ptrL < pixVal)
//										censusL |= (1 << cnt);
//									ptrL++;
//									cnt++;
//								}
//								ptrL += (m_width - 1) * m_maxDisp;
//							}
//							unsigned char pixValR = *imgRightPtr;
//							unsigned char* ptrR = imgRightPtr - distToStart;
//
//							cnt = 0;
//							for (int i = 0; i < 5; i++) {
//								for (int j = 0; j < 5; j++) {
//									if (*ptrR < pixValR)
//										censusR |= (1 << cnt);
//									ptrR++;
//									cnt++;
//								}
//								ptrR += (m_width - 1) * m_maxDisp;
//							}
//							int delta = censusL ^ censusR;
//							short dist = 0;
////							LOG_FORMAT_INFO("Delta %d", delta);
//							while (delta) {
//								++dist;
//								delta &= delta - 1;
//							}
////							LOG_FORMAT_INFO("Dist %d", dist);
//							*costPtr++ = dist;
//						}
//						if (x - d < 0)
//							*costPtr++ = 255;// Sehr hohe kosten TODO: Richtig ?
//						else {
//							// TODO imgRightPtr nicht immer neu berechnen
//							*costPtr++ = abs(
//									(*imgLeftPtr) - *(imgRightPtr - d));// = |L(y,x) - R(y,x-d)|
//						}
//					} else {
//						if (x + d >= m_width)
//							*costPtr++ = 255; // Sehr hohe kosten TODO: Richtig ?
//						else {
//							// TODO imgRightPtr nicht immer neu berechnen
//							*costPtr++ = abs(
//									(*imgRightPtr) - *(imgLeftPtr + d)); // = |L(y,x-d) - R(y,x)|
//						}
//					}
//				}
//				imgLeftPtr++;
//				imgRightPtr++;
//			}
//			imgLeftPtr+=4;
//			imgRightPtr+=4;
//			costPtr+=4*m_maxDisp;
//		}
		// TODO Funktioniert nicht
	}else if(imgLeft->getFormat() == Image::PF_RGB_888) {
		for (int y = 0; y < m_height; y++) {
			unsigned short* costPtr = m_CostData + m_width * m_maxDisp * y;
			unsigned char* imgLeftPtr = imgLeft->getPtr<unsigned char>(y);
			unsigned char* imgRightPtr = imgRight->getPtr<unsigned char>(y);
			for (int x = 0; x < m_width; x++) {
				for (int d = 0; d < m_maxDisp; d++) {

					if (m_Left2Right) {
						if (x - d < 0)
							*costPtr++ = 0xffff; // Sehr hohe kosten TODO: Richtig ?
						else {
							*costPtr++ = sqrt(
									((*(imgLeftPtr + 0)) - *(imgRightPtr - d*3 + 0))*((*(imgLeftPtr + 0)) - *(imgRightPtr - d*3 + 0)) +
									((*(imgLeftPtr + 1)) - *(imgRightPtr - d*3 + 1))*((*(imgLeftPtr + 1)) - *(imgRightPtr - d*3 + 1)) +
									((*(imgLeftPtr + 2)) - *(imgRightPtr - d*3 + 2))*((*(imgLeftPtr + 2)) - *(imgRightPtr - d*3 + 2))

							); // = |L(y,x) - R(y,x-d)|
						}
					} else {
						if (x + d >= m_width)
							*costPtr++ = 0xffff; // Sehr hohe kosten TODO: Richtig ?
						else {
							*costPtr++ = sqrt(
									((*(imgRightPtr + 0)) - *(imgLeftPtr - d*3 + 0))*((*(imgRightPtr + 0)) - *(imgLeftPtr - d*3 + 0)) +
									((*(imgRightPtr + 1)) - *(imgLeftPtr - d*3 + 1))*((*(imgRightPtr + 1)) - *(imgLeftPtr - d*3 + 1)) +
									((*(imgRightPtr + 2)) - *(imgLeftPtr - d*3 + 2))*((*(imgRightPtr + 2)) - *(imgLeftPtr - d*3 + 2))

							);
						}
					}
				}
				imgLeftPtr+=3;
				imgRightPtr+=3;
			}
		}
	}
	// Scale to 11 bits
	unsigned short max = *max_element(m_CostData, m_CostData + m_dataSize);

	unsigned short* costPtr = m_CostData;
	for (int i = 0; i < m_dataSize; i++) {
		costPtr[i] = costPtr[i] * 0x3ff / max;
	}
}

void SGM::aggregateCost(){

	int addOneX = m_maxDisp;
	int addOneY = m_width*m_maxDisp;

	memset(m_aggregatedCosts,0,m_buffSize);
	memset(m_tmpAggrCost,0,(3*m_width*m_maxDisp + m_maxDisp)*sizeof(short));

	// Offsets into buffer
	int offs1 = 0;
	int offs2 = m_width*m_maxDisp;
	int offs3 = 2*m_width*m_maxDisp;
	int offs4 = 3*m_width*m_maxDisp;

	int offs5 = 3*m_width*m_maxDisp;
	int offs6 = 0;
	int offs7 = m_width*m_maxDisp;
	int offs8 = 2*m_width*m_maxDisp;

	unsigned short tmpBuff[4][m_maxDisp];
	unsigned short tmpBuff2[m_maxDisp];

	memcpy(&m_tmpAggrCost[offs1],m_CostData,m_width*m_maxDisp*sizeof(short));
	memcpy(&m_tmpAggrCost[offs2],m_CostData,m_width*m_maxDisp*sizeof(short));
	memcpy(&m_tmpAggrCost[offs3],m_CostData,m_width*m_maxDisp*sizeof(short));

	int costIdx = addOneY;
	int aggrGlobalIdx = addOneY;
	int IdxPrevX;
	int IdxCurrX;

	for (int i = 0; i < addOneY; i++) {
		m_aggregatedCosts[i] += 4*m_CostData[i];
	}

	for(int y = 1; y < m_height; y++){
		// Init first x from cost buffer for path 4
		memcpy(&m_tmpAggrCost[offs4],&m_CostData[costIdx],m_maxDisp*sizeof(short));

		// Copy initial value to results
		for (int i = 0; i < addOneX; i++) {
			m_aggregatedCosts[aggrGlobalIdx + i] += 4*m_CostData[costIdx + i];
		}
		costIdx+= addOneX; 		// Skip x = 0;
		aggrGlobalIdx+= addOneX;
		for(int x = 1; x < m_width; x++){
#pragma omp parallel sections num_threads(4)
		{
#pragma omp section
			{
			int IdxCurrX = offs1 + (x) * addOneX;
			int IdxPrevX = offs1 + (x-1) * addOneX;
			// Calculate result and store in tmpBuff
			evaluatePath(&m_tmpAggrCost[IdxPrevX], &m_CostData[costIdx], tmpBuff[0]);

//			for (int i = 0; i < addOneX; i++) {
//				m_aggregatedCosts[aggrGlobalIdx + i] += tmpBuff[0][i];
//			}
			// Use a second temp buffer. Wen can swap buffers directly because the old values are used in the next step
			memcpy(&m_tmpAggrCost[IdxPrevX],tmpBuff2,m_maxDisp*sizeof(short));
			memcpy(tmpBuff2,tmpBuff[0],m_maxDisp*sizeof(short));
			}

#pragma omp section
			{
			int IdxPrevX = offs2 + x * addOneX;
			int IdxCurrX = offs2 + (x) * addOneX;
			// Calculate result and store in tmpBuff
			evaluatePath(&m_tmpAggrCost[IdxPrevX], &m_CostData[costIdx], tmpBuff[1]);

//			for (int i = 0; i < addOneX; i++) {
//				m_aggregatedCosts[aggrGlobalIdx + i] += tmpBuff[1][i];
//			}
			memcpy(&m_tmpAggrCost[IdxCurrX],tmpBuff[1],m_maxDisp*sizeof(short));
			}
#pragma omp section
			{
			int IdxCurrX = offs3 + x * addOneX;
			int IdxPrevX = offs3 + (x + 1) * addOneX;
			// Calculate result and store in tmpBuff
			evaluatePath(&m_tmpAggrCost[IdxPrevX], &m_CostData[costIdx], tmpBuff[2]);

//			for (int i = 0; i < addOneX; i++) {
//				m_aggregatedCosts[aggrGlobalIdx + i] += tmpBuff[2][i];
//			}
			memcpy(&m_tmpAggrCost[IdxCurrX],tmpBuff[2],m_maxDisp*sizeof(short));
			}

#pragma omp section
			{
			int IdxCurrX = offs4;
			int IdxPrevX = offs4;
			// Calculate result and store in tmpBuff
			evaluatePath(&m_tmpAggrCost[IdxPrevX], &m_CostData[costIdx], tmpBuff[3]);
//			for (int i = 0; i < addOneX; i++) {
//				m_aggregatedCosts[aggrGlobalIdx + i] += tmpBuff[3][i];
//			}
			memcpy(&m_tmpAggrCost[IdxCurrX],tmpBuff[3],m_maxDisp*sizeof(short));
			}
				}
			}

			// All all 4 values
			for (int i = 0; i < addOneX; i++) {
				m_aggregatedCosts[aggrGlobalIdx + i] += tmpBuff[0][i] +  tmpBuff[1][i] +  tmpBuff[2][i] +  tmpBuff[3][i];
			}

			costIdx+= addOneX;
			aggrGlobalIdx+= addOneX;
	}
	memcpy(&m_tmpAggrCost[offs6], m_CostData,	m_width * m_maxDisp * sizeof(short));
	memcpy(&m_tmpAggrCost[offs7], m_CostData,	m_width * m_maxDisp * sizeof(short));
	memcpy(&m_tmpAggrCost[offs8], m_CostData,	m_width * m_maxDisp * sizeof(short));

	costIdx = m_width*m_height*m_maxDisp - addOneY;
	aggrGlobalIdx = m_width*m_height*m_maxDisp - addOneY;

	for (int i = 0; i < addOneY; i++) {
		m_aggregatedCosts[aggrGlobalIdx + i] += 4 * m_CostData[costIdx + i];
	}

	for (int y = m_height-2; y >= 0; y--) {
		// Init first x from cost buffer for path 4
		memcpy(&m_tmpAggrCost[offs4], &m_CostData[costIdx], m_maxDisp * sizeof(short));

		// Copy initial value to results
		for (int i = 0; i < addOneX; i++) {
			m_aggregatedCosts[aggrGlobalIdx + i] += 4*m_CostData[costIdx + i];
		}

		costIdx -= addOneX; 		// Skip x = 0;
		aggrGlobalIdx -= addOneX;
		for (int x = m_width-2; x >= 0; x--) {
#pragma omp parallel sections num_threads(4)
		{
#pragma omp section
			{
			int IdxCurrX = offs8 + (x) * addOneX;
			int	IdxPrevX = offs8 + (x + 1) * addOneX;
			// Calculate result and store in tmpBuff
			evaluatePath(&m_tmpAggrCost[IdxPrevX], &m_CostData[costIdx], tmpBuff[0]);

			for (int i = 0; i < addOneX; i++) {
				m_aggregatedCosts[aggrGlobalIdx + i] += tmpBuff[0][i];
			}
			// Use a second temp buffer. Wen can swap buffers directly because the old values are used in the next step
			memcpy(&m_tmpAggrCost[IdxPrevX], tmpBuff2, m_maxDisp * sizeof(short));
			memcpy(tmpBuff2, tmpBuff[0], m_maxDisp * sizeof(short));
			}

#pragma omp section
			{
			int IdxPrevX = offs7 + x * addOneX;
			int IdxCurrX = offs7 + x * addOneX;
			// Calculate result and store in tmpBuff
			evaluatePath(&m_tmpAggrCost[IdxPrevX], &m_CostData[costIdx], tmpBuff[1]);

			for (int i = 0; i < addOneX; i++) {
				m_aggregatedCosts[aggrGlobalIdx + i] += tmpBuff[1][i];
			}
			memcpy(&m_tmpAggrCost[IdxCurrX], tmpBuff[1], m_maxDisp * sizeof(short));
			}

#pragma omp section
			{
			int IdxCurrX = offs6 + x * addOneX;
			int IdxPrevX = offs6 + (x - 1) * addOneX;
			// Calculate result and store in tmpBuff
			evaluatePath(&m_tmpAggrCost[IdxPrevX], &m_CostData[costIdx], tmpBuff[2]);

			for (int i = 0; i < addOneX; i++) {
				m_aggregatedCosts[aggrGlobalIdx + i] += tmpBuff[2][i];
			}
			memcpy(&m_tmpAggrCost[IdxCurrX], tmpBuff[2], m_maxDisp * sizeof(short));
			}

#pragma omp section
			{
			int IdxCurrX = offs5;
			int IdxPrevX = offs5;
			// Calculate result and store in tmpBuff
			evaluatePath(&m_tmpAggrCost[IdxPrevX], &m_CostData[costIdx], tmpBuff[3]);
			for (int i = 0; i < addOneX; i++) {
				m_aggregatedCosts[aggrGlobalIdx + i] += tmpBuff[3][i];
			}
			memcpy(&m_tmpAggrCost[IdxCurrX], tmpBuff[3], m_maxDisp * sizeof(short));
			}
		}
			costIdx -= addOneX;
			aggrGlobalIdx -= addOneX;
		}
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

//#pragma omp parallel for
	unsigned int idx = 0;
	for (int i = 0; i < m_height * m_width; i++) {

		unsigned short* aggCostPtr = &m_aggregatedCosts[idx];
		// Index is Disparity
		m_disparityMap[i] = min_element(aggCostPtr, aggCostPtr+m_maxDisp)- aggCostPtr;

		idx+= m_maxDisp;
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
			int offsRight = floor(dispL + 0.5);

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

				if(dispR <0 || abs((int)(dispL - dispR)) > threshold)
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
			int offsetLeft = floor(dispR + 0.5);;

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

				if(dispL <0 || abs((int)(dispL - dispR)) > threshold)
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
