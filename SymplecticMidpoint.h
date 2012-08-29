#pragma once

#include "IIntegrator.h"

template <typename T>
class SymplecticMidpoint: public IIntegrator<T>
{
public:
	void operator()(const vec3<Real>& posIn, const vec3<Real>& velIn, AccelerationDelegate<T>& accDel,const Real& dTime, vec3<Real>& posOut, vec3<Real>& velOut)
	{
		vec3<Real> x1f2, v1f2;
		v1f2 = velIn + 0.5 * dTime * accDel(posIn, velIn);
		x1f2 = posIn + 0.5* dTime * v1f2;

		velOut = velIn + dTime * accDel(x1f2, v1f2);
		posOut =  posIn + dTime * velOut;
	}
};