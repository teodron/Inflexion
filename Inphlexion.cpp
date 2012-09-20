// Inphlexion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DummyDLO.h"
#include "AccelerationDelegate.h"

#include "Integrators.h"
#include "TypeDefs.h"

#include "World.h"

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

// GRAPHICS
shared_ptr<World> world;
void Display()
{
	world->Render();
}
void Reshape(int w, int h)
{
	world->Reshape(w, h);
}
void Keyboard(unsigned char key, int x, int y)
{
	world->Keyboard(key, x, y);
}

int _tmain(int argc, char* argv[])
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

	//(*euler)(pos, vel, *po->GetDelegate(), dTime, npos, nvel);

	po->SetIntegrationMethod(IPhysicalObject::IntegrationMethod::EULER_RICHARDSON);

	euler = po->GetIntegrator();

	(*euler)(pos, vel, *po->GetDelegate(), dTime, npos, nvel);

	cout<<length(npos)<<" "<<length(nvel);
	
	cin.get();

	
	world.reset(new World(argc, argv));
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutIdleFunc(Display);
	glutMainLoop();

	return 0;
}

