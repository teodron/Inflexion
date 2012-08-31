#include "stdafx.h"
#include "IPhysicalObject.h" 
#include "Integrators.h"
#include "TypeDefs.h"

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