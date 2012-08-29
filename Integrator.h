#pragma  once
#include "IIntegrator.h"

template <typename T>
class Integrator : public IIntegrator<T>
{
public:
	void operator()(const vec3<Real>& posIn, const vec3<Real>& velIn, AccelerationDelegate<T>& accDel,const Real& dTime,
		vec3<Real>& posOut, vec3<Real>& velOut)
	{
		posOut = posIn + dTime * velIn;
		vec3<Real> acc = accDel(posOut, velIn);
		velOut = velIn + dTime * acc;
		
	}
};
