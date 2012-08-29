#pragma once
#include "IPhysicalObject.h"
#include "idelegate.h"

//! Acceleration delegate class
/*!
	This templated delegate is used in conjunction with an object's acceleration 
	computation method and further facilitates interaction with a numerical integration
	provider.
*/
template <typename T>
class AccelerationDelegate: public IDelegate
{
	// The T-type object can access the privates of an acceleration delegate since
	// it holds an instance used to pass to an integrator.
	friend T;
private:

	//! A pointer to the object that supplies the acceleration method.
	T* objInstance;

	//! Pointer to the non-static acceleration method.
	vec3<Real> (T::*mPtr)(const vec3<Real>& pos, const vec3<Real>& vel);

public:
	//! Custom constructor.
	/*!
		\param t is an instance of the acceleration supplier object.
		\param fnPtr is the pointer to the non-static method.
	*/
	AccelerationDelegate(T& t, vec3<Real> (T::*fnPtr)(const vec3<Real>& pos, const vec3<Real>& vel))
	{
		objInstance = &t;
		mPtr = fnPtr;
	}
	//! Empty destructor
	~AccelerationDelegate() {} 

	//! Overloaded function call operator.
	/*!
		Considering an object to consist of mass points/particles that are subjected to
		certain constraints, each particle has a subsequent acceleration accessible through
		this delegate. This acceleration is used by different generic integrators.
		\param pos the input position.
		\param vel the input velocity.
		\return the acceleration given the position and velocity of a particle.
	*/
	vec3<Real> operator()(const vec3<Real>& pos, const vec3<Real>& vel)
	{
		return (objInstance->*mPtr)(pos,vel);
	}
};



