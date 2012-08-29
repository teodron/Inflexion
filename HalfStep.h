#pragma once
#include "IIntegrator.h"

template <typename T>
class HalfStep: public IIntegrator<T>
{
public:
	void operator()(const vec3<Real>& posIn, const vec3<Real>& velIn, AccelerationDelegate<T>& accDel,const Real& dTime, vec3<Real>& posOut, vec3<Real>& velOut)
	{
		// x(n+1/2) v(n+1/2)
		vec3<Real> x1f2, v1f2;
		x1f2 = posIn + 0.5 * dTime * velIn;
		v1f2 = velIn + 0.5 * dTime * accDel(posIn, velIn);

		posOut = posIn + dTime * v1f2;
		velOut = velIn + dTime * accDel(x1f2, v1f2);
	}
};