#pragma once

#include "IPhysicalObject.h"
#include "GL\freeglut.h"

class TorsionDLO : public IPhysicalObject
{
private:


	//! The acceleration of the current particle of this DLO
	vec3<Real> Acceleration(const vec3<Real>& pos,const vec3<Real>& vel);

	GLUquadric* cylinder;

public:

	//! Default empty constructor
	TorsionDLO(): IPhysicalObject()
	{
		cylinder = gluNewQuadric();
	}
	//! Custom constructor
	TorsionDLO(const vector<vec3<Real> >& points, const Real& radius, const Real& mass): IPhysicalObject(points, radius, mass)
	{
		cylinder = gluNewQuadric();
	}

	//! empty destructor.
	~TorsionDLO() 
	{
		gluDeleteQuadric(cylinder);
	}

	void Render();
};