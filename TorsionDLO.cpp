
#include "stdafx.h"
#include "TorsionDLO.h"

// render function
void TorsionDLO::Render()
{
	for (int i = 0; i < nPoints- 1; ++i)
	{
		glPushMatrix();
		glTranslatef(points[i].r.x,points[i].r.y,points[i].r.z);
		vec3<Real> dir = points[i+1].r - points[i].r;
		Real len = length((dir));
		if (len > 1e-6)
			dir /= len;
		Real alpha = 180 / M_PI * atan2(sqrt(dir.x * dir.x + dir.y * dir.y), dir.z);
		Real beta = 180 / M_PI * atan2(dir.y, dir.x);
		glRotatef(beta, 0, 0, 1);
		glRotatef(alpha, 0, 1, 0);
		gluCylinder(cylinder, rad, rad, len, 10, 2);
		glPopMatrix();
	}
}
// The acceleration function
vec3<Real> TorsionDLO::Acceleration(const vec3<Real>& pos,const vec3<Real>& vel)
{
	vec3<Real> force = zeroVec;
	// linear spring force
	// P_{i} - P_{i+1}
	vec3<Real> ei = zeroVec, eim1 = zeroVec;
	if (currentPointIndex < nPoints - 1)
	{
		Real lr = length(points[currentPointIndex + 1].r0 - points[currentPointIndex].r0);
		Real li = length(points[currentPointIndex + 1].r - points[currentPointIndex].r);
		Real DeltaL = (li - lr) / lr;
		ei =  (points[currentPointIndex+1].r - points[currentPointIndex].r) / li;
		force += this->Kl * DeltaL * ei;
	}
	// P_{i-1} - P_{i}
	if (currentPointIndex > 0)
	{
		Real lr = length(points[currentPointIndex - 1].r0 - points[currentPointIndex].r0);
		Real lim1 = length(points[currentPointIndex - 1].r - points[currentPointIndex].r);
		Real DeltaL = (lim1 - lr) / lr;
		eim1 = (points[currentPointIndex].r - points[currentPointIndex - 1].r) / lim1;
		force += - this->Kl * DeltaL * eim1;
	}

	// linear damper
	Real viTilde = 0, vip1Tilde = 0, vim1Tilde = 0;
	if (currentPointIndex < nPoints - 1)
	{
		viTilde = dot(vel, ei);
		vip1Tilde = dot(points[currentPointIndex +1].v, ei);
		force += this->Kd * (vip1Tilde - viTilde) * ei;
	}
	if (currentPointIndex > 0)
	{
		viTilde = dot(vel, eim1);
		vim1Tilde = dot(points[currentPointIndex - 1].v, eim1);
		force += this->Kd * (vim1Tilde - viTilde) * eim1;
	}

	// torsion spring force
	vec3<Real> tip1 = cross(ei, cross(eim1, ei));
	vec3<Real> tim1 = cross(eim1, cross(eim1, ei));
	tip1 = normalize(tip1);
	tim1 = normalize(tim1);
	vec3<Real> eiRest = zeroVec, eim1Rest = zeroVec;
	if (currentPointIndex < nPoints - 1 && currentPointIndex > 0)
	{
		eiRest = points[currentPointIndex + 1].r0 - points[currentPointIndex].r0;
		eiRest = normalize(eiRest);
		eim1Rest = points[currentPointIndex].r0 - points[currentPointIndex - 1].r0;
		eim1Rest = normalize(eim1Rest);

		Real alphaiRest = acos(dot(eim1Rest, eiRest));
		Real alphai = acos(dot(eim1, ei));
		Real DeltaAlpha = alphai - alphaiRest;
		force += -this->Kts * DeltaAlpha * tim1 / length(points[currentPointIndex].r - points[currentPointIndex - 1].r);
		force += -this->Kts * DeltaAlpha * tip1 / length(points[currentPointIndex + 1].r - points[currentPointIndex].r);
	}

	// torsional damper force
	if (currentPointIndex < nPoints -1 && currentPointIndex > 0)
	{
		Real vim1, vip1, vib, via;
		vim1 = dot(points[currentPointIndex - 1].v, tim1);
		vip1 = dot(points[currentPointIndex +1].v, tip1);
		vib = dot(vel, tim1);
		via = dot(vel, tip1);
		Real velocityFactor = ((vim1 - vib) / length(points[currentPointIndex].r - points[currentPointIndex - 1].r)
			                 + (vip1 - via) / length(points[currentPointIndex + 1].r - points[currentPointIndex].r)) * Ktd;
		vec3<Real> fim1 = velocityFactor * tim1 / length(points[currentPointIndex - 1].r - points[currentPointIndex].r);
		vec3<Real> fip1 = velocityFactor * tip1 / length(points[currentPointIndex + 1].r - points[currentPointIndex].r);
		force += fim1 + fip1;
	}

	// swivel damper force
	vec3<Real> s = cross(eim1, ei);
	if (currentPointIndex > 0)
	{
		force += this->Ksw * dot((points[currentPointIndex - 1].v - vel), s) / length(points[currentPointIndex - 1].r - points[currentPointIndex].r) * s;
	}
	if (currentPointIndex < nPoints -1)
	{
		force += this->Ksw * dot((points[currentPointIndex + 1].v - vel), s) / length(points[currentPointIndex + 1].r - points[currentPointIndex].r) * s;
	}
	vec3<Real> acceleration =  force / points[currentPointIndex].mass;
	// add gravity
	acceleration += vec3<Real>(0, 0, -gravity);
	// add external force contributions and viscous drag
	acceleration += points[currentPointIndex].df / points[currentPointIndex].mass - bDamping * vel / mass;

	return acceleration;
}
