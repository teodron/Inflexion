#pragma once

#include "IPhysicalObject.h"

class TorsionDLO : public IPhysicalObject
{
private:


	//! The acceleration of the current particle of this DLO
	vec3<Real> Acceleration(const vec3<Real>& pos,const vec3<Real>& vel);

public:

	//! Default empty constructor
	TorsionDLO(): IPhysicalObject()
	{

	}
	//! Custom constructor
	TorsionDLO(const vector<vec3<Real> >& points, const Real& radius, const Real& mass): IPhysicalObject(points, radius, mass)
	{

	}

	//! empty destructor.
	~TorsionDLO() 
	{

	}
	/*!
	\todo complete this function with additional synchronization and residue updates
	*/
	void Update();


	void Render();
};