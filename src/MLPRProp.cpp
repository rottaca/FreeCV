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

	// Init weights and rates
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

void MLPRProp::evalMLP(const Vectord& X,  Vectord *y2, Vectord* y1,  Vectord* u1, Vectord* u2)
{
	// Init output vectors
	y1->init(m_h+1);
	u1->init(m_h+1);
	y2->init(m_p);
	u2->init(m_p);

	// Hidden layer output
	y1[0] = -1;	// Bias
	for(int i = 0; i < m_h;i++)
	{
		u1[i] = -m_W1.at(i,0);		// Bias : (-1)*w0

		for(int j = 0; j < m_n;j++)
		{
			u1[i+1] += X[j]*m_W1.at(i,j+1);
		}

		y1[i+1] = m_trans->eval((*u1)[i]);
	}

	// Output layer output
	for(int i = 0; i < m_p;i++)
	{
		u2[i] = -m_W2.at(i,0);
		for(int j = 1; j < m_h+1;j++)
		{
			u2[i] += y1[j]*m_W2.at(i,j);
		}

		y2[i] = m_trans->eval((*u2)[i]);
	}
}
void MLPRProp::evalMLP(const Matrixd X, int dataIdx, Vectord& y1, Vectord &y2, Vectord& u1, Vectord& u2)
{
	// Init output vectors
	y1.init(m_h+1);
	u1.init(m_h+1);
	y2.init(m_p);
	u2.init(m_p);

	// Hidden layer output
	y1[0] = -1;	// Bias
	u1[0] = 0;	// This value does not exist

	for(int i = 0; i < m_h;i++)
	{
		u1[i+1] = -m_W1.at(i,0);		// Bias : (-1)*w0

		for(int j = 0; j < m_n;j++)
		{
			u1[i+1] += X.at(dataIdx,j)*m_W1.at(i,j+1);
		}

		y1[i+1] = m_trans->eval(u1[i]);
	}

	// Output layer output
	for(int i = 0; i < m_p;i++)
	{
		u2[i] = -m_W2.at(i,0);
		for(int j = 1; j < m_h+1;j++)
		{
			u2[i] += y1[j]*m_W2.at(i,j);
		}

		y2[i] = m_trans->eval(u2[i]);
	}
}
double MLPRProp::calcErrorMLP(const Matrixd& X, const Matrixd& T)
{
	int cnt = T.getRows();
	double error = 0;
	Vectord u1,u2,y1,y2,delta(m_p);
	// For each training sample
	for(int t = 0; t < cnt;t++)
	{
		// Evaluate Neuron
		evalMLP(X,t,y1,y2,u1,u2);
		// Calculate delta
		for(int i = 0; i < m_p;i++)
		{
			delta[i] = y2[i]-T.at(t,i);
		}
		// Calculate euclidian error
		double e = delta.getLength();
		error += e*e;
	}
	return error;
}
double MLPRProp::trainMLP(const Matrixd& X, const Matrixd& T)
{
	double error = 0;
	int it = 0;

	// Storage for update aggregation
	Matrixd upd1 = Matrixd(m_h,m_n+1);
	Matrixd upd2 = Matrixd(m_p,m_h+1);

	Vectord u1,u2,y1,y2,delta1(m_h),delta2(m_p);

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
			for(int i = 0; i < m_p;i++)
			{
				delta2[i] = (T.at(t,i)-y2[i])*m_trans->eval(u2[i],1);
			}
			// Calculate delta1
			for(int i = 0; i < m_h;i++)
			{
				delta1[i] = 0;
				for(int j=0;j<m_p;j++){
					delta1[i] += delta2[j]*m_W2.at(j,i);
				}
				delta1[i] *= m_trans->eval(u1[i],1);
			}

			// Accumulate Update W2
			for (int i = 0; i < m_h;i++)
			{
				for (int j = 0; j < m_p;j++)
				{
					upd2.at(j,i) += y1[i]*delta2[j];
				}
			}
			// Accumulate Update W1
			for (int k = 0; k < m_n;k++)
			{
				for (int i = 0; i < m_h;i++)
				{
					upd1.at(i,k) += X.at(t,k)*delta1[i];
				}
			}
		}

		// Adjust weights
		for (int i = 0; i < m_h;i++)
		{
			for (int j = 0; j < m_p;j++)
			{
				m_W2.at(j,i) += m_R2.at(j,i)*upd2.at(j,i);
			}
		}
		for (int k = 0; k < m_n;k++)
		{
			for (int i = 0; i < m_h;i++)
			{
				m_W1.at(i,k) += m_R1.at(i,k)*upd1.at(i,k);
			}
		}

		// Adjust teach rates TODO

		// Calculate squared error
		error = calcErrorMLP(X,T);
		LOG_FORMAT_INFO("[RPORP] Error: %f",error);

	} while (it <= m_maxIt && error > m_eps);	// Break when iteration limit reached or when error small enough



	return error;
}

} /* namespace fcv */
