#pragma once
#include "IPhysicalObject.h"

class DummyDLO : public IPhysicalObject
{
public:
	DummyDLO(){}
	~DummyDLO(){}
	 vec3<Real> Acceleration(const vec3<Real>& pos,const vec3<Real>& vel)
	 {
		 cout<<" Dummy acceleration ";
		 return pos + vel;
	 }

	 AccelerationDelegate<IPhysicalObject>* GetDelegate()
	 {
		 return &this->accDel;
	 }

	 void Update()
	 {

	 }
};