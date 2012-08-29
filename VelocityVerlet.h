#pragma once
#include "IIntegrator.h"

template <typename T>
class VelocityVerlet: public IIntegrator<T>
{
public:
	void operator()(const vec3<Real>& posIn, const vec3<Real>& velIn, AccelerationDelegate<T>& accDel,const Real& dTime, vec3<Real>& posOut, vec3<Real>& velOut)
	{
		vec3<Real> f = accDel(posIn, velIn);
		posOut = posIn + dTime * velIn + 0.5 * dTime * dTime * f;
		vec3<Real> vTilde = velIn + 0.5 * dTime * f;
		velOut = velIn + 0.5 * ( f+ accDel(posOut, vTilde));
	}
};