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

void MassPoint::CollisionUtilities::CollideLinks(MassPoint * Pi, MassPoint * Qi, MassPoint * Ri,
	MassPoint * Pip1, MassPoint * Qip1, MassPoint * Rip1,
	MassPoint * Pj, MassPoint * Qj, MassPoint * Rj,
	MassPoint * Pjp1, MassPoint * Qjp1, MassPoint * Rjp1, const Real& rad, const Real& mu)
{
	vec3<Real> Gi = 0.333 * (Pi->rPlus + Qi->rPlus + Ri->rPlus);
	vec3<Real> Gip1 = 0.333 * (Pip1->rPlus + Qip1->rPlus + Rip1->rPlus);
	vec3<Real> Gj = 0.333 * (Pj->rPlus + Qj->rPlus + Rj->rPlus);
	vec3<Real> Gjp1 = 0.333 * (Pjp1->rPlus + Qjp1->rPlus + Rjp1->rPlus);
	// lerp parameters
	Real uu, vv;
	GeometricUtilities<Real>::DistanceBetweenSegments(Gi, Gip1, Gj, Gjp1, uu, vv);
	vec3<Real> C = Gi + uu * (Gip1 - Gi);
	vec3<Real> E = Gj + vv * (Gjp1 - Gj);
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
		vec3<Real> FGi = 0.333 * (Pi->f + Qi->f + Ri->f);
		vec3<Real> FGip1 = 0.333 * (Pip1->f + Qip1->f + Rip1->f);
		vec3<Real> FGj = 0.333 * (Pj->f + Qj->f + Rj->f);
		vec3<Real> FGjp1 = 0.333 * (Pjp1->f + Qjp1->f + Rjp1->f);
		vec3<Real> Fc = (1 - uu) * FGi + uu * FGip1;
		vec3<Real> Fe = (1 - vv) * FGj + vv * FGjp1;
		vec3<Real> N = vec3<Real>(0,0,0); // reaction force
		if (Real dotProduct = dot(Fe - Fc, n) >= 1e-6)
		{
			N = dotProduct * n;
		}
		//velocities
		vec3<Real> vGi = 0.333 * (Pi->vPlus + Qi->vPlus + Ri->vPlus);
		vec3<Real> vGip1 = 0.333 * (Pip1->vPlus + Qip1->vPlus + Rip1->vPlus);
		vec3<Real> vGj = 0.333 * (Pj->vPlus + Qj->vPlus + Rj->vPlus);
		vec3<Real> vGjp1 = 0.333 * (Pjp1->vPlus + Qjp1->vPlus + Rjp1->vPlus);
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
		Pi->dr += di; Qi->dr += di; Ri->dr += di;
		Pip1->dr += dip1; Qip1->dr += dip1; Rip1->dr += dip1;
		Pj->dr += dj; Qj->dr += dj; Rj->dr += dj;
		Pjp1->dr += djp1; Qjp1->dr += djp1; Rjp1->dr += djp1;
		// add velocities
		// i
		vec3<Real> vel = (1 - uu) * iPrime * n;
		Pi->vRes += vel / Pi->mass; Qi->vRes += vel / Qi->mass; Ri->vRes += vel / Ri->mass;
		// i+1
		vel = uu * iPrime * n;
		Pip1->vRes += vel / Pip1->mass; Qip1->vRes += vel / Qip1->mass; Rip1->vRes += vel / Rip1->mass;
		// j
		vel = - (1 - vv) * iPrime * n;
		Pj->vRes += vel / Pj->mass; Qj->vRes += vel / Qj->mass; Rj->vRes += vel / Rj->mass;
		// j+1
		vel = - vv * iPrime * n;
		Pjp1->vRes += vel / Pjp1->mass; Qjp1->vRes += vel / Qjp1->mass; Rjp1->vRes += vel / Rjp1->mass;
		// add forces
		// i
		vec3<Real> force = (1 - uu) / sigma * (N + Ffriction);
		Pi->df += force; Qi->df += force; Ri->df += force;
		// i+1
		force = uu / sigma * (N +  Ffriction);
		Pip1->df += force; Qip1->df += force; Rip1->df += force;
		// j 
		force = - (1 - vv) / sigma * (N + Ffriction);
		Pj->df += force; Qj->df += force; Rj->df += force;
		// j+1
		force = - vv / sigma * (N + Ffriction);
		Pjp1->df += force; Qjp1->df += force; Rjp1->df += force;

	}



}