/*
 * IMLPTransfer.h
 *
 *  Created on: 25.06.2016
 *      Author: andreas
 */

#ifndef FREECV_INCLUDE_FREECV_NEURO_IMLPTRANSFER_H_
#define FREECV_INCLUDE_FREECV_NEURO_IMLPTRANSFER_H_

namespace fcv {
class IMLPTransfer {
public:
	virtual ~IMLPTransfer(){}
	/**
	 * Calculates the axonal potential or its deviation
	 */
	virtual double eval(double u, int nthDiff=0) = 0;

};
}


#endif /* FREECV_INCLUDE_FREECV_NEURO_IMLPTRANSFER_H_ */
