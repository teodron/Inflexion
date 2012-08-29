#pragma once
template <typename T>
class RungeKutta4: public IIntegrator<T>
{
public:
	void operator()(const vec3<Real>& posIn, const vec3<Real>& velIn, AccelerationDelegate<T>& accDel,const Real& dTime, vec3<Real>& posOut, vec3<Real>& velOut)
	{
		vec3<Real> x1, v1, x2, v2, x3, v3, x4, v4, a1, a2, a3, a4;
		x1 = posIn;
		v1 = velIn;
		a1 =  accDel(x1, v1);

		x2 = posIn + 0.5 * dTime * v1;
		v2 = velIn + 0.5 * dTime * a1;
		a2 = accDel(x2, v2);

		x3 = posIn + 0.5 * dTime * v2;
		v3 = velIn + 0.5 * dTime * a2;
		a3 = accDel(x3, v3);

		x4 = posIn + dTime * v3;
		v4 = velIn + dTime * a3;
		a4 = accDel(x4, v4);

		posOut = posIn + 0.16666 * (v1 + 2*v2 + 2*v3 + v4);
		velOut = velIn + 0.16666 * (a1 + 2*a2 + 2*a3 + a4);
	}
};