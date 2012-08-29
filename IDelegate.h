#pragma once
#include "stdafx.h"

//! Delegate interface.
/*!
	This is a delegate interface aimed at wrapping acceleration 
	computation functions. 
*/
class IDelegate
{

protected:
	//! Empty constructor.
	IDelegate(){}

	//! Empty virtual destructor.
	virtual ~IDelegate() {}

	//! Pure virtual functor.
	/*!
		This delegate is agnostic of the source of the acceleration function.
		\param pos the position of a particle.
		\param vel the velocity of a particle.
		\return the acceleration of the particle.
	*/
	virtual vec3<Real> operator()(const vec3<Real>& pos, const vec3<Real>& vel) = 0;
};