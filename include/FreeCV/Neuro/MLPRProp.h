/*
 * MLPRProp.h
 *
 *  Created on: 25.06.2016
 *      Author: andreas
 */

#ifndef FREECV_INCLUDE_FREECV_NEURO_MLPRPROP_H_
#define FREECV_INCLUDE_FREECV_NEURO_MLPRPROP_H_

#include "FreeCV/Core/Vector.h"
#include "FreeCV/Core/Matrix.h"


namespace fcv {

class IMLPTransfer;

class MLPRProp {
public:
	MLPRProp();
	virtual ~MLPRProp();

	/**
	 * Initializes a neuronal MLP network, trained with RPROP
	 * a - initialisation value range [-a,a]
	 * rateStart - teach rate
	 * ratep - rate+
	 * ratem - rate-
	 * epsilon - teaching will quit when squared error is smaller than this
	 * value
	 * maxEpoche - maximum Epoche if error is not small enough
	 * f - transfer function and derivation interface
	 * n, h, p - Input, hidden and output layer size
	 */
	bool initMLP(double a,
			double rateStart, double ratep, double ratem, double rateMax, double rateMin,
			double eps, double maxIt,int n,int h,int p, IMLPTransfer* trans);

	double trainMLP(const Matrixd& X, const Matrixd& T);

	/**
	 * Calculates the overall error between the neuronal network and the training signal
	 */
	double calcErrorMLP(const Matrixd& X, const Matrixd& T);
	/**
	 * Evaluates the neuronal network with with a given input vector
	 * It outputs the output for each neuron in the hidden and output layer
	 * @param X Input Vector
	 * @param y1 Output of the hidden layer
	 * @param y2 Output of the output layer
	 * @param u1
	 */
 	void evalMLP(const Vectord& X,  Vectord *y2, Vectord* y1=NULL,  Vectord* u1=NULL, Vectord* u2=NULL);

private:
	/**
	 * Evaluates the neuronal network with all InputData (Matrix), and the dataIdx to select a single data sample.
	 * It outputs the output for each neuron in the hidden and output layer
	 * @param X Input Matrix with a single row for each input sample
	 * @param dataIdx Index into the DataMatrix X to select an input for evaluation
	 * @param y1 Output of the hidden layer
	 * @param y2 Output of the output layer
	 * @param u1
	 */
 	void evalMLP(const Matrixd X, int dataIdx, Vectord& y1, Vectord &y2, Vectord& u1, Vectord &u2);
private:

	double m_a;
	double m_rateStart;
	double m_ratep;
	double m_ratem;
	double m_rateMax;
	double m_rateMin;

	double m_eps;
	double m_maxIt;
	int m_n;
	int m_h;
	int m_p;

	IMLPTransfer* m_trans;

	// Weights and teach rates
	Matrixd m_W1,m_W2,m_R1,m_R2;

};

} /* namespace fcv */

#endif /* FREECV_INCLUDE_FREECV_NEURO_MLPRPROP_H_ */
