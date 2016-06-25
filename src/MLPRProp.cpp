/*
 * MLPRProp.cpp
 *
 *  Created on: 25.06.2016
 *      Author: andreas
 */

#include "FreeCV/Neuro/MLPRProp.h"
#include "FreeCV/Neuro/IMLPTransfer.h"

#include "FreeCV/Core/Math.h"
namespace fcv {


MLPRProp::MLPRProp() {
	m_trans = NULL;
}

MLPRProp::~MLPRProp() {

}

bool MLPRProp::initMLP(double a,
			double rateStart, double ratep, double ratem, double rateMax, double rateMin,
			double eps, double maxIt,int n,int h,int p,IMLPTransfer* trans)
{
	m_a = a;
	m_rateStart=rateStart;
	m_ratep=ratep;
	m_ratem=ratem;
	m_rateMax=rateMax;
	m_rateMin=rateMin;

	m_eps=eps;
	m_maxIt=maxIt;
	m_n=n;
	m_h=h;
	m_p=p;
	m_trans = trans;

	// Init Weigths and rates
    // Indexing in matrices is as follows
    // W(a,b): Perceptron a, weight b-1 of perceptron a, b = 0 -> bias
	m_W1 = Matrixd(h,n+1);
	m_R1 = Matrixd(h,n+1);
	m_W2 = Matrixd(p,h+1);
	m_R2 = Matrixd(p,h+1);

	for(int i = 0; i <h;i++)
	{
		for(int j = 0; j <n+1;j++)
		{
			m_W1.at(i,j) = FCV_FRAND(-m_a,m_a);
			m_R1.at(i,j) = m_rateStart;
		}
	}
	for(int i = 0; i <p;i++)
	{
		for(int j = 0; j <h+1;j++)
		{
			m_W2.at(i,j) = FCV_FRAND(-m_a,m_a);
			m_R2.at(i,j) = m_rateStart;
		}
	}


	return true;
}

void MLPRProp::evalMLP(Matrixd X, int dataIdx, Vectord& y1, Vectord &y2, Vectord& u1, Vectord &u2)
{
	// Init output vectors
	y1.init(m_h+1);
	y2.init(m_p);

	// Hidden layer output
	y1[0] = -1;	// Bias
	for(int i = 1; i < m_h+1;i++)
	{
		u1[i] = -m_W1.at(i,0);		// Bias : (-1)*w0

		for(int j = 1; j < m_n;j++)
		{
			u1[i] += X.at(dataIdx,j-1)*m_W1.at(i,j);
		}

		y1[i] = m_trans->eval(u1[i]);
	}

	// Output layer output
	for(int i = 0; i < m_p;i++)
	{
		u2[i] = 0;
		for(int j = 0; j < m_h+1;j++)
		{
			u2[i] += y1[j]*m_W2.at(i,j);
		}

		y2[i] = m_trans->eval(u2[i]);
	}
}
double MLPRProp::calcErrorMLP(Matrixd X, Matrixd T)
{
	int cnt = T.getRows();
	double error = 0;
	Vectord u1,u2,y1,y2,delta(T.getCols());
	// For each training sample
	for(int t = 0; t < cnt;t++)
	{
		// Evaluate Neuron
		evalMLP(X,t,y1,y2,u1,u2);
		// Calculate delta
		for(int i = 0; i < T.getCols();i++)
		{
			delta[i] = y2[i]-T.at(t,i);
		}
		// Calculated euclidian error
		error += delta.getLength();
	}
	return error;
}
double MLPRProp::trainMLP(Matrixd X, Matrixd T)
{
	double error = 0;
	int it = 0;

	// Storage for update aggregation
	Matrixd upd1 = Matrixd(m_h,m_n+1);
	Matrixd upd2 = Matrixd(m_p,m_h+1);

	Vectord u1,u2,y1,y2,delta1(T.getCols()),delta2(T.getCols());

	do {

		Matrixd upd1Last = upd1;
		Matrixd upd2Last = upd2;
		Matrixd upd1 = Matrixd(m_h,m_n+1);
		Matrixd upd2 = Matrixd(m_p,m_h+1);

		// For each training sample
		for(int t = 0; t < X.getRows();t++)
		{
			// Evaluate Neuron
			evalMLP(X,t,y1,y2,u1,u2);
			// Calculate delta2
			for(int i = 0; i < T.getCols();i++)
			{
				delta2[i] = T.at(t,i)-y2[i]*m_trans->eval(u2[i],1);
			}
			// Calculate delta1

		}

	} while (it <= m_maxIt && error > m_eps);	// Break when iteration limit reached or when error small enought



	return error;
}

} /* namespace fcv */
