/*
 * SGM.h
 *
 *  Created on: 28.10.2015
 *      Author: andreas
 */

#ifndef SGM_H_
#define SGM_H_

#include "FreeCV/Image/Image.h"
#include <vector>
namespace fcv {

class SGM {
public:
	SGM();
	virtual ~SGM();

	static bool l2rConsistencyCheck(Image* dispLeft, Image* dispRight, int threshold);

	bool processImagePair(Image* imgLeft, Image* imgRight, bool Left2Right = true);

	bool init(int width, int height, int maxDisp);
	void deinit();

	unsigned short* getCostData(){
		return m_CostData;
	}
	float* getDisparityData(){
		return m_disparityMap;
	}

	void updatePenalties(unsigned short p1, unsigned short p2){
		m_penalty1 = p1;
		m_penalty2 = p2;
	}

	long calcExpectedMemoryUsage(){
		return (2*m_dataSize+2*m_width*m_maxDisp + m_maxDisp)*sizeof(short) + m_width*m_height*sizeof(float);
	}

private:
	typedef struct Path{
		int x;
		int y;
		Path(int x, int y):
			x(x), y(y){	}
	} Path;

	void calculateCost(Image* imgLeft, Image* imgRight);
	void aggregateCost();
	void computeDisparityMap();

	void initAggregateCostDir(Path p);
	inline void evaluatePath(unsigned short* priorAccPtr, unsigned short* currCostPtr, unsigned short* currentAccPtr);

private:
	unsigned short* m_CostData;
	float* m_disparityMap;
	unsigned short* m_aggregatedCosts;
	unsigned short* m_tmpAggrCost;

	int m_width,m_height, m_maxDisp;
	size_t m_dataSize, m_buffSize;
	unsigned short m_penalty1, m_penalty2;
	bool m_isInitialized;
	std::vector<Path> m_paths;

	bool m_Left2Right;

};

} /* namespace fcv */

#endif /* SGM_H_ */
