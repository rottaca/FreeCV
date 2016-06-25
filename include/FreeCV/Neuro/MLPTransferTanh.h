/*
 * MLPTransferTanh.h
 *
 *  Created on: 25.06.2016
 *      Author: andreas
 */

#ifndef FREECV_INCLUDE_FREECV_NEURO_MLPTRANSFERTANH_H_
#define FREECV_INCLUDE_FREECV_NEURO_MLPTRANSFERTANH_H_

#include <math.h>


namespace fcv {

class MLPTransferTanh : public IMLPTransfer{
public:
	virtual ~MLPTransferTanh(){}

	double eval(double u, int nthDiff)
	{
		switch (nthDiff) {
		case 0:
			return tanh(u);
		case 1:{
			double c = cosh(u);
			return 1/(c*c);
		}
		default:
			return 0;
		}
	}
};

} /* namespace fcv */

#endif /* FREECV_INCLUDE_FREECV_NEURO_MLPTRANSFERTANH_H_ */
