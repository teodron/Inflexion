#include "stdafx.h"
#include "IPhysicalObject.h" 
#include "Integrators.h"
#include "TypeDefs.h"
#include "GeometricUtilities.h"

IPhysicalObject::IPhysicalObject(const vector<vec3<Real> >& points, const Real& radius, const Real& mass):
									accDel(*this, &IPhysicalObject::Acceleration), rad(radius), mass(mass),
										integrationStrategy(0)
 {
	 nPoints = points.size();
	 // reserve mass point vectors
	 this->points.resize(nPoints);
	 for (int idx = 0; idx < nPoints; ++idx)
	 {
		 this->points[idx] = MassPoint(points[idx]);
		 this->points[idx].SetMass(mass / nPoints);
	 }

	 this->SetIntegrationMethod(integrationStrategy);
 }


void IPhysicalObject::SetIntegrationMethod(int methodType)
{
	switch (methodType)
	{
		case IntegrationMethod::EXPLICIT_EULER:
			integrator = ExplicitEulerSingleton::GetInstance();
		break;

		case IntegrationMethod::SYMPLECTIC_EULER:
			integrator = SymplecticEulerSingleton::GetInstance();
		break;

		case IntegrationMethod::MIDPOINT:
			integrator = MidpointSingleton::GetInstance();
		break;

		case IntegrationMethod::HALF_STEP:
			integrator = HalfStepSingleton::GetInstance();
		break;

		case IntegrationMethod::EULER_RICHARDSON:
			integrator = EulerRichardsonSingleton::GetInstance();
		break;

		case IntegrationMethod::SYMPLECTIC_MIDPOINT:
			integrator = SymplecticMidpointSingleton::GetInstance();
		break;

		case IntegrationMethod::VELOCITY_VERLET:
			integrator = VelocityVerletSingleton::GetInstance();
		break;

		case IntegrationMethod::POSITION_VERLET:
		default:
			integrator = IntegratorSingleton::GetInstance();
		break;
	}
}

void IPhysicalObject::Integrate()
{
	for (currentPointIndex = 0; currentPointIndex < nPoints; ++currentPointIndex)
	{
		if (integrationStrategy != IPhysicalObject::IntegrationMethod::POSITION_VERLET)
			(*integrator)(points[currentPointIndex].r, points[currentPointIndex].v, accDel, dTime, points[currentPointIndex].rPlus, points[currentPointIndex].vPlus);
		else
			(*integrator)(points[currentPointIndex].r, points[currentPointIndex].v, points[currentPointIndex].rMinus, accDel, dTime, points[currentPointIndex].rPlus, points[currentPointIndex].vPlus);
	}
}

void IPhysicalObject::HandleSelfCollision()
{
	for (size_t i = 0; i < nPoints -1; ++i)
	{
		for (size_t j = i + 2; j < nPoints - 1; ++j)
		{
			MassPoint::CollisionUtilities::CollideLinks(&points[i], &points[i+1], &points[j], &points[j+1], rad, mu);
		}
	}
}

void IPhysicalObject::ComputeConstraintContributions()
{
	for (size_t i = 0; i < nPoints - 1; ++i)
	{
		// pi - pi+1
		Real currLength = length(points[i].rPlus - points[i+1].rPlus);
		Real restLength = length(points[i].r0 - points[i+1].r0);
		vec3<Real> displacement = - this->lengthConstraintFraction * (points[i].rPlus - points[i+1].rPlus)/ currLength * ( currLength - restLength);
		points[i].dr += displacement;
		points[i+1].dr += -displacement;
	}
}


void IPhysicalObject::ComputeForces()
{
	for (vector<MassPoint>::iterator it = points.begin(); it != points.end(); ++it)
	{
		it->f = (it->vPlus - it->v) / dTime;
	}
}
void IPhysicalObject::ResetDisplacements()
{
	for (vector<MassPoint>::iterator it = points.begin(); it != points.end(); ++it)
	{
		it->ResetDisplacement();
	}
}

void IPhysicalObject::ResetRestitutionVelocities()
{
	for (vector<MassPoint>::iterator it = points.begin(); it != points.end(); ++it)
	{
		it->ResetRestitutionVelocity();
	}
}

void IPhysicalObject::ResetForces()
{
	for (vector<MassPoint>::iterator it = points.begin(); it != points.end(); ++it)
	{
		it->ResetForce();
	}
}

void IPhysicalObject::ResetAllAccumulators()
{
	for (vector<MassPoint>::iterator it = points.begin(); it != points.end(); ++it)
	{
		it->ResetDisplacement();
		it->ResetRestitutionVelocity();
		it->ResetForce();
	}
}

void IPhysicalObject::ComputeCorrectedPositions()
{
	for (vector<MassPoint>::iterator it = points.begin(); it != points.end(); ++it)
	{
		it->CorrectPosition();
	}
}

void IPhysicalObject::ComputeCorrectedVelocities()
{
	for (vector<MassPoint>::iterator it = points.begin(); it != points.end(); ++it)
	{
		it->CorrectVelocity();
	}
}

void IPhysicalObject::SynchronizePositionsAndVelocities()
{
	for (vector<MassPoint>::iterator it = points.begin(); it != points.end(); ++it)
	{
		it->SynchronizePositionsAndVelocities();
	}
}

void IPhysicalObject::Update()
{
	// compute new positions and velocities
	Integrate();

	// approximate forces
	ComputeForces();

	
	//Apply position constraints (length-preserving)
	ResetDisplacements();
	ComputeCorrectedPositions();

	// HandleCollisions
	ResetAllAccumulators();
	HandleSelfCollision();
	ComputeCorrectedPositions();
	ComputeCorrectedVelocities();

	// Synchronize (copy state(t) to state(t - deltaT) and state(t + deltaT) to state(t) 
	SynchronizePositionsAndVelocities();

}