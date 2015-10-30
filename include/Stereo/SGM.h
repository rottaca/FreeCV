/*
 * SGM.h
 *
 *  Created on: 28.10.2015
 *      Author: andreas
 */

#ifndef SGM_H_
#define SGM_H_

#include "Image/Image.h"
#include <vector>
namespace fcv {

class SGM {
public:
	SGM();
	virtual ~SGM();

	bool processImagePair(Image* imgLeft, Image* imgRight);

	bool init(int width, int height, int maxDisp);
	void deinit();

	unsigned int* getCostData(){
		return m_CostData;
	}
	float* getDisparityData(){
		return m_disparityMap;
	}

	void updatePenalties(unsigned int p1, unsigned int p2){
		m_penalty1 = p1;
		m_penalty2 = p2;
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
	void evaluatePath(unsigned int* priorAccPtr, unsigned int* currCostPtr, unsigned int* currentAccPtr);


private:
	unsigned int* m_CostData;
	float* m_disparityMap;
	unsigned int* m_aggregatedCosts;
	unsigned int* m_aggregatedCostsDir;

	int m_width,m_height, m_maxDisp;
	unsigned int m_penalty1, m_penalty2;
	bool m_isInitialized;
	std::vector<Path> m_paths;

};

} /* namespace fcv */

#endif /* SGM_H_ */
