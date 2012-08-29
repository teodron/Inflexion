#pragma once
#include "stdafx.h"

template <typename Numeric>
	class GeometricUtilities
	{
	public:
		static void DistanceBetweenSegments(vec3<Numeric> p0, vec3<Numeric> p1, vec3<Numeric> q0, vec3<Numeric> q1,Numeric &sc, Numeric &tc)
		{
			vec3 <Numeric> u=p1-p0;
			vec3 <Numeric> v=q1-q0;
			vec3 <Numeric> w=p0-q0;
			Numeric a=dot(u,u);
			Numeric b=dot(u,v);
			Numeric c=dot(v,v);
			Numeric d=dot(u,w);
			Numeric e=dot(v,w);
			Numeric D=a*c-b*b;
			Numeric sn,sd=D;
			Numeric tn,td=D;
			Numeric eps = static_cast<Numeric>(0.000001);
			if (D<eps)
			{
				sn=0;sd=1;tn=e;td=c;
			}
			else
			{
				sn=b*e-c*d;tn=a*e-b*d;
				if (sn<0)
				{
					sn=0;tn=e;td=c;
				}
				else
					if (sn>sd)
					{
						sn=sd;tn=e+b;td=c;
					}
			}
			if (tn<0)
			{
				tn=0;
				if (-d<0)
					sn=0;
				else
					if (-d>a)
						sn=sd;
					else
					{
						sn=-d;
						sd=a;
					}
			}
			else
				if (tn>td)
				{
					tn=td;
					if ((-d+b)<0)
						sn=0;
					else
						if ((-d+b)>a)
							sn=sd;
						else
						{
							sn=(-d+b);
							sd=a;
						}
				}
				if (fabs(sn)<eps) sc=0; else sc=sn/sd;
				if (fabs(tn)<eps) tc=0; else tc=tn/td;
				//vec3 <double> pc,qc;
				//pc=p0+sc*u;
				//qc=q0+tc*v;
				//cout<<pc.x<<" "<<pc.y<<" "<<pc.z<<endl;
				//cout<<qc.x<<" "<<qc.y<<" "<<qc.z<<endl;

		}
	protected:

		GeometricUtilities(void);
		~GeometricUtilities(void);
	};


