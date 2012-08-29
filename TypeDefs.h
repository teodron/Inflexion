#pragma once

typedef TSingleton<Integrator<IPhysicalObject> > IntegratorSingleton;

typedef TSingleton<ExplicitEuler<IPhysicalObject> > ExplicitEulerSingleton;

typedef TSingleton<SymplecticEuler<IPhysicalObject> > SymplecticEulerSingleton;

typedef TSingleton<Midpoint<IPhysicalObject> > MidpointSingleton;

typedef TSingleton<HalfStep<IPhysicalObject> > HalfStepSingleton;

typedef TSingleton<EulerRichardson<IPhysicalObject> > EulerRichardsonSingleton;

typedef TSingleton<VelocityVerlet<IPhysicalObject> > VelocityVerletSingleton;

typedef TSingleton<SymplecticMidpoint<IPhysicalObject> > SymplecticMidpointSingleton;

typedef TSingleton<RungeKutta4<IPhysicalObject> > RungeKutta4Singleton;