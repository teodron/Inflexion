#pragma once
#include "stdafx.h"
#include "AccelerationDelegate.h"
#include "TSingleton.h"
//! Templated integrator interface.
/*!
	This interface establishes a protocol for classes that implement various integration 
	methods.
*/
template <typename T>
class IIntegrator
{
public:
	//! Pure virtual functor operator
	/*!
		\param posIn the input position of a particle
		\param velIn the input velocity of a particle
		\param accDel the acceleration delegate corresponding to the object type containing the particle
		\param dTime the time step used for integration
		\param posOut the output position resulting after one integration step
		\param velOut the output velocity as a result of one integration step
	*/
	virtual void operator()(const vec3<Real>& posIn, const vec3<Real>& velIn, AccelerationDelegate<T>& accDel,const Real& dTime,
					vec3<Real>& posOut, vec3<Real>& velOut) = 0;

	//! Default integration method operator: Position Verlet
	/*!
		There is no reason to make this method virtual since only the position Verlet uses this signature.

		\param posIn the input position of a particle
		\param velIn the input velocity of a particle
		\param prevPosIn the previous input particle position
		\param accDel the acceleration delegate corresponding to the object type containing the particle
		\param dTime the time step used for integration
		\param posOut the output position resulting after one integration step
		\param velOut the output velocity as a result of one integration step
	*/
	void operator()(const vec3<Real>& posIn, const vec3<Real>& velIn, const vec3<Real>& prevPosIn, AccelerationDelegate<T>& accDel,const Real& dTime, vec3<Real>& posOut, vec3<Real>& velOut)
	{
		posOut = 2.0 * posIn - prevPosIn + accDel(posIn, velIn) * dTime * dTime;
		velOut = (posOut - prevPosIn) / (2.0 * dTime);
	}
};