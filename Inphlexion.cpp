// Inphlexion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DummyDLO.h"
#include "AccelerationDelegate.h"

#include "Integrators.h"
#include "TypeDefs.h"

class Delegate
{
protected:
	Delegate(){}
	virtual ~Delegate(){}
	virtual vec3<Real> operator()(const vec3<Real>& pos, const vec3<Real>& vel) = 0;
};

template <typename T>
class ADelegate: public IDelegate
{
private:
	T* objInstance;
	vec3<Real> (T::*mPtr)(const vec3<Real>& pos, const vec3<Real>& vel);
public:
	ADelegate(T& t, vec3<Real> (T::*fnPtr)(const vec3<Real>& pos, const vec3<Real>& vel))
	{
		objInstance = &t;
		mPtr = fnPtr;
	}
	~ADelegate() {} 

	 vec3<Real> operator()(const vec3<Real>& pos, const vec3<Real>& vel)
	 {
		 return (objInstance->*mPtr)(pos, vel);
	 }
};

void Function(ADelegate<IPhysicalObject>& ad)
{
	vec3<Real> a(0.,0.,1.), b(0.,0.,2.0);
	vec3<Real> result = ad(a,b);
	cout<<" Function .. "<< length(result);
}

void AccFun(AccelerationDelegate<IPhysicalObject>& ad)
{
	vec3<Real> a(0.,0.,1.), b(0.,0.,2.0);
	vec3<Real> result = ad(a,b);
	cout<<" accfun .. "<< length(result);
}
class Hell
{
public:
	Hell()
	{
		cout<<"Hell";
	}
};
//typedef TSingleton<Integrator<IPhysicalObject> > IntegratorSingleton;

int _tmain(int argc, _TCHAR* argv[])
{
	IPhysicalObject* po = new DummyDLO();
	ADelegate<IPhysicalObject> ad(*po, &IPhysicalObject::Acceleration);
	AccelerationDelegate<IPhysicalObject> acd(*po, &IPhysicalObject::Acceleration);
	//Function(ad);
	AccFun(*po->GetDelegate());

	//IIntegrator<IPhysicalObject>* euler = Integrator<IPhysicalObject>::GetInstance();
	IIntegrator<IPhysicalObject>* euler = IntegratorSingleton::GetInstance();
	/*
	Integrator<IPhysicalObject> euler;// = Singleton<Integrator<IPhysicalObject> >::Instance();
	CSingleton<Hell>::Instance();
	CSingleton<Hell>::Instance();
	CSingleton<Hell>::Instance();
	*/
	vec3<Real> pos(0,0,0), vel(1.,1.,1.);
	Real dTime = 0.01;
	vec3<Real> npos, nvel;
	(*euler)(pos, vel, *po->GetDelegate(), dTime, npos, nvel);

	cout<<length(npos)<<" "<<length(nvel);
	
	cin.get();
	return 0;
}

