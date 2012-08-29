#pragma once
#include "IIntegrator.h"

template <typename T>
class EulerRichardson: public IIntegrator<T>
{
public:
	void operator()(const vec3<Real>& posIn, const vec3<Real>& velIn, AccelerationDelegate<T>& accDel,const Real& dTime, vec3<Real>& posOut, vec3<Real>& velOut)
	{
		vec3<Real> xMid, vMid;
		vMid = velIn + 0.5 * dTime * accDel(posIn, velIn);
		xMid = posIn + 0.5 * dTime * velIn;

		velOut = velIn + dTime * accDel(xMid, vMid);
		posOut = posIn + dTime * vMid;
	}
};