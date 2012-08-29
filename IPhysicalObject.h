#pragma once
#include "stdafx.h"
#include "AccelerationDelegate.h"
#include "MassPoint.h"

class IPhysicalObject
{
protected:
	//! Number of mass points the object consists of.
	int nPoints; 

	/// the radius of this model ( the circumscribed circle radius for each right RQP triangle)
	Real rad;
	/// the total DLO mass
	Real mass;
	/// a fraction describing how much a length preserving constraint uses of the residual displacement
	Real lengthConstraintFraction;
	/// linear spring stiffness constant
	Real Kl;
	/// torsion spring stiffness constant
	Real Kt;
	/// Damping coefficient.
	Real bDamping;

	//! Vector of mass points that make up a conceptual physical object.
	vector<MassPoint> points;

	//! Acceleration delegate member.
	AccelerationDelegate<IPhysicalObject> accDel;

	IPhysicalObject(void): accDel(*this, &IPhysicalObject::Acceleration)
	{
	}

	IPhysicalObject(const vector<vec3<Real> >& points, const Real& radius, const Real& mass);

	virtual ~IPhysicalObject(void)
	{
	}

public:

	//! Acceleration function.
	/*!
		\param pos the position of a particle .
		\param vel the velocity of the particle.
		\return the acceleration vector of a particle that is internally set as the current particle through a specific state variable.
	*/
	virtual vec3<Real> Acceleration(const vec3<Real>& pos,const vec3<Real>& vel) = 0;

	//! Updates the dynamics and kinematics of the body
	/*!
		Pure virtual method symbolizing an update step of the entire physical object via
		numerical integration.
	*/
	virtual void Update() = 0;

	//! Acceleration delegate getter.
	/*!
		\return the delegate wrapped acceleration method
	*/
	virtual AccelerationDelegate<IPhysicalObject>* GetDelegate() = 0;

	//! Resets the mass point displacement accumulators.
	void ResetDisplacements();

	//! Resets the mass point offset velocity accumulators.
	void ResetRestitutionVelocities();

	//! Resets the mass point force offset accumulators.
	void ResetForces();

	//! sets the linear spring constant.
	void SetKl(Real value)
	{
		Kl = value;
	}

	//! sets the torsion/angular spring constant
	void SetKt(Real value)
	{
		Kt = value;
	}

	//! sets the fraction used to accumulate length preserving displacements
	void SetLengthConstraintFraction(Real value)
	{
		lengthConstraintFraction = value;
	}

	//! sets the energy loss/damping coefficient
	void SetDampingCoefficient(Real value)
	{
		bDamping = value;
	}

};

