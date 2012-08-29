#pragma once
#include "stdafx.h"
#include "AccelerationDelegate.h"
#include "MassPoint.h"

#include "IIntegrator.h"

class IPhysicalObject
{
public:
	enum IntegrationMethod
	{
		EXPLICIT_EULER,
		SYMPLECTIC_EULER,
		MIDPOINT,
		HALF_STEP,
		EULER_RICHARDSON,
		VELOCITY_VERLET,
		POSITION_VERLET,
		SYMPLECTIC_MIDPOINT,
		RUNGE_KUTTA_4
	};

protected:

	//! The chosen integration method (strategy pattern)
	int integrationStrategy;

	//! Integration strategy/method
	IIntegrator<IPhysicalObject> * integrator;

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

	//! default empty constructor.
	IPhysicalObject(void): accDel(*this, &IPhysicalObject::Acceleration), integrationStrategy(0)
	{
		this->SetIntegrationMethod(integrationStrategy);
	}

	//! Custom constructor
	/*!
		\param points represents a vector of points that describe the initial positions of the body's particles
		\param radius is the radius of the object along its longest dimension
		\param mass the total body mass
	*/
	IPhysicalObject(const vector<vec3<Real> >& points, const Real& radius, const Real& mass);

	//! empty virtual destructor
	virtual ~IPhysicalObject(void)
	{
	}

public:

	void SetIntegrationMethod(int methodType);

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

	IIntegrator<IPhysicalObject>* GetIntegrator()
	{
		return integrator;
	}

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

