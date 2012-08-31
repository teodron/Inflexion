#include "StdAfx.h"
#include "MassPoint.h"
#include "GeometricUtilities.h"


MassPoint::MassPoint()
{

}
MassPoint::MassPoint(vec3<Real> pos)
{
	// initialize
	r = r0 = rPlus = rMinus = pos;
	v = v0 = vPlus = vMinus = zeroVec;
	dr = f = vRes = df = zeroVec;
}

MassPoint::MassPoint(const MassPoint& src)
{
	// copy 
	r = src.r; r0 = src.r0; rMinus = src.rMinus; rPlus = src.rPlus;
	v = src.v; v0 = src.v0; vMinus = src.vMinus; vPlus = src.vPlus;
	dr = src.dr; f = src.f; vRes = src.vRes; df = src.df;
}

void MassPoint::SynchronizePositionsAndVelocities()
{
	// t - Dt <- t
	rMinus = r;
	vMinus = v;
	// t <- t + Dt
	r = rPlus;
	v = vPlus;
}
MassPoint::~MassPoint(void)
{
}

void MassPoint::CollisionUtilities::CollideLinks(MassPoint * Pi, MassPoint * Pip1,
												 MassPoint * Pj, MassPoint * Pjp1,
												 const Real& rad, const Real& mu)
{
	// lerp parameters
	Real uu, vv;
	GeometricUtilities<Real>::DistanceBetweenSegments(Pi->rPlus, Pip1->rPlus, Pj->rPlus, Pjp1->rPlus, uu, vv);
	vec3<Real> C = Pi->rPlus + uu * (Pip1->rPlus - Pi->rPlus);
	vec3<Real> E = Pj->rPlus + vv * (Pjp1->rPlus - Pj->rPlus);
	Real dist = length(C - E);
	// if the distance is less than 2*rad, a collision occurred
	if (dist > 2e-6 && dist - 2 *  rad < 0)
	{
		// displacements
		Real fraction = (2 * rad / dist - 1.0);
		vec3<Real> di = (1 - uu) * fraction * (C - E);
		vec3<Real> dip1 = uu * fraction * (C - E);
		vec3<Real> dj = - (1 - vv) *  fraction * (C - E);
		vec3<Real> djp1 = - vv * fraction * (C - E);
		// forces
		vec3<Real> n = (C - E) / dist; // collision axis
		vec3<Real> FGi = Pi->f ;
		vec3<Real> FGip1 = Pip1->f ;
		vec3<Real> FGj = Pj->f ;
		vec3<Real> FGjp1 = Pjp1->f ;
		vec3<Real> Fc = (1 - uu) * FGi + uu * FGip1;
		vec3<Real> Fe = (1 - vv) * FGj + vv * FGjp1;
		vec3<Real> N = vec3<Real>(0,0,0); // reaction force
		if (Real dotProduct = dot(Fe - Fc, n) >= 1e-6)
		{
			N = dotProduct * n;
		}
		//velocities
		vec3<Real> vGi = Pi->vPlus ;
		vec3<Real> vGip1 = Pip1->vPlus;
		vec3<Real> vGj = Pj->vPlus;
		vec3<Real> vGjp1 = Pjp1->vPlus;
		vec3<Real> vC = (1 - uu) * vGi + uu * vGip1; // velocity at C
		vec3<Real> vE = (1 - vv) * vGj + vv * vGjp1; // velocity at E
		vec3<Real> vRel = vC - vE; // relative velocity
		vec3<Real> e = dot(vRel, n) * n - vRel;
		if (Real len = length(e) > 1e-5) 
			e /= len; // the friction direciton
		vec3<Real> impulse = N * dTime; // segment collision impulse
		Real sigma = uu*uu + vv*vv + (1-uu)*(1-uu) + (1-vv)*(1-vv);
		vec3<Real> Ffriction = mu * length(N) * e;
		Real iPrime = 2 * length(impulse) / sigma;
		// add displacements
		Pi->dr += di; 
		Pip1->dr += dip1; 
		Pj->dr += dj;
		Pjp1->dr += djp1; 
		// add velocities
		// i
		vec3<Real> vel = (1 - uu) * iPrime * n; // impulse
		Pi->vRes += vel / Pi->mass; 
		// i+1
		vel = uu * iPrime * n;
		Pip1->vRes += vel / Pip1->mass;
		// j
		vel = - (1 - vv) * iPrime * n;
		Pj->vRes += vel / Pj->mass;
		// j+1
		vel = - vv * iPrime * n;
		Pjp1->vRes += vel / Pjp1->mass;
		// add forces
		// i
		vec3<Real> force = (1 - uu) / sigma * (N + Ffriction);
		Pi->df += force; 
		// i+1
		force = uu / sigma * (N +  Ffriction);
		Pip1->df += force;
		// j 
		force = - (1 - vv) / sigma * (N + Ffriction);
		Pj->df += force; 
		// j+1
		force = - vv / sigma * (N + Ffriction);
		Pjp1->df += force; 

	}



}