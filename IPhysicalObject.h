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

	//! The index of the current point/particle that is being updated.
	int currentPointIndex;

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
	/// Linear damping coefficient
	Real Kd;
	/// Torsion spring coefficient
	Real Kts;
	/// Torsion damper coefficient
	Real Ktd;
	/// Swivel damper coefficient
	Real Ksw;
	/// Damping coefficient.
	Real bDamping;
	/// friction coefficient
	Real mu;

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

	

public:

	/************************************************************************/
	/* Render                                                               */
	/************************************************************************/

	//! Render method stub. Each object must provide a render function.
	virtual void Render() = 0;

	/************************************************************************/
	/* Physics                                                              */
	/************************************************************************/

	//! Acceleration function.
	/*!
		\param pos the position of a particle .
		\param vel the velocity of the particle.
		\return the acceleration vector of a particle that is internally set as the current particle through a specific state variable.
	*/
	virtual vec3<Real> Acceleration(const vec3<Real>& pos,const vec3<Real>& vel) = 0;

	//! Integrates numerically all points to find the parameters of the next time instance.
	void Integrate();

	//! Updates the dynamics and kinematics of the body
	/*!
		Virtual method corresponding to an update step of the entire physical object via
		numerical integration.
	*/
	virtual void Update();

	//! Computes restitution velocities, friction and reaction forces in case the object collides with itself
	/*!
		The points of the object are updated later on with the aid of the information
		stored in their accumulators for each Physics related parameter (i.e.
		displacements, velocities and forces)
	*/
	virtual void HandleSelfCollision();

	//! Position-based dynamics projected constraints
	/*!
		For further details, consult the works of Provot, Jakobsen and any relevant
		literature on position-based dynamics. The way these constraints are implemented
		reflect an iterative process that has been proved to converge for most cases.
	*/
	virtual void ComputeConstraintContributions();

	//! Computes the forces after an integration step via finite differencing
	void ComputeForces();

	/************************************************************************/
	/* Getters/Setters                                                      */
	/************************************************************************/

	//! Sets the integration method (using a value from the enum containing the options)
	void SetIntegrationMethod(int methodType);

	//! Acceleration delegate getter.
	/*!
		\return the delegate wrapped acceleration method
	*/
	virtual AccelerationDelegate<IPhysicalObject>* GetDelegate()
	{
		return &this->accDel;
	}

	//! Returns the current integrator this object uses for updates
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

	//! Resets all accumulators (displacement, velocity and force)
	void ResetAllAccumulators();

	//! Corrects the positions adding accumulated displacements
	void ComputeCorrectedPositions();

	//! Corrects the new velocities adding accumulated velocity offsets
	void ComputeCorrectedVelocities();

	//! Copies the new values into the previous value holders (both velocity and position vectors)
	void SynchronizePositionsAndVelocities();

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

	//! sets the linear damper coefficient
	void SetKd(Real value)
	{
		Kd = value;
	}
	
	//! sets the torsion spring coefficient
	void SetKts(Real value)
	{
		Kts = value;
	}

	//! sets the torsion damper coefficient
	void SetKtd(Real value)
	{
		Ktd = value;
	}
	//! sets the swivel damper coefficient
	void SetKsw(Real value)
	{
		Ksw = value;
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

	//! empty virtual destructor
	virtual ~IPhysicalObject(void)
	{

	}
};

