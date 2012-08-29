#include "IIntegrator.h" 

template <typename T>
class ExplicitEuler: public IIntegrator<T>
{
public:
	void operator()(const vec3<Real>& posIn, const vec3<Real>& velIn, AccelerationDelegate<T>& accDel,const Real& dTime, vec3<Real>& posOut, vec3<Real>& velOut)
	{
		posOut = posIn + dTime * velIn;
		velOut = velIn + dTime * accDel(posIn, velIn);
	}
};