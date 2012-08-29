#include "stdafx.h"
#include "IPhysicalObject.h" 

IPhysicalObject::IPhysicalObject(const vector<vec3<Real> >& points, const Real& radius, const Real& mass):
									accDel(*this, &IPhysicalObject::Acceleration), rad(radius), mass(mass)
 {
	 nPoints = points.size();
	 // reserve mass point vectors
	 this->points.resize(nPoints);
	 for (int idx = 0; idx < nPoints; ++idx)
	 {
		 this->points[idx] = MassPoint(points[idx]);
		 this->points[idx].SetMass(mass / nPoints);
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