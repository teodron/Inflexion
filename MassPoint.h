#pragma once
#include "stdafx.h"
#include <assert.h>

	/**
	* \class MassPoint
	* \brief Provides support for a 3D point endowed with a mass
	* 
	* A mass point is described by its position, mass, force and velocity
	* vectors. For easier compatibility with various integration methods,
	* this class also stores the position and velocity of this point
	* at three consecutive time instances: t - Dt, t and t + Dt,
	* where Dt represents the integration time step.
	* Support for collision handling or constraint reinforcement is
	* provided through the aid of a displacements vector dr, while
	* the restitution velocities are stored separately in vRest. 
	* Friction and reaction/restitution forces are accumulated 
	* as usual in the forces vector f.
	*/
	class MassPoint
	{
		friend ostream& operator<<(ostream& output, const MassPoint& p);

		//! TorsionDLO is made of mass points, hence it is allowed to access its privates.
		friend class TorsionDLO;

		friend class IPhysicalObject;

	private:
		/// positions
		vec3<Real> r, rMinus, rPlus, r0;
		/// velocities
		vec3<Real> v, vMinus, vPlus, v0;
		/// collision and constraints contributions
		vec3<Real> vRes, f, dr, df;

		Real mass; //< the mass concetration at this point (in kilograms)
	public:
		/************************************************************************/
		/* Collision utilities                                                  */
		/************************************************************************/

		/**
		* \brief Utility class providing routines that perform collision detection and resolution
		* \author Teodor Cioaca
		* \date August 2012
		*/
		static class CollisionUtilities
		{
			// friend class VolumetricDOO;
		private:
			/**
			* \brief Perform collision detection and handling between two DLO segments
			* The function accumulates collision response forces (reaction and friction), restitution velocities (from impulses)
			* and displacement penalties (from projected constraints). Collisions are handled asynchronously for all colliding
			* segment pairs. Each pair consists of a capped cylindrical tube.

			* \param Pi - endpoint of cylinder i
			* \param Pip1 - other endpoint of cylinder i

			* \param Pj - endpoint of cylinder j
			* \param Pjp1 - other endpoint of cylinder j

			* \param rad - the radius of the volumetric segment
			* \param mu - friction coefficient
			*/
			static void CollideLinks(MassPoint * Pi,  MassPoint * Pip1,
							  MassPoint * Pj,  MassPoint * Pjp1,
							  const Real & rad, const Real& mu);
		};

		/// Creates a static mass point at a specified position
		MassPoint(vec3<Real> pos);
		/// copy constructor
		MassPoint(const MassPoint& src);
		/// empty default constructor
		MassPoint();
		/// mass setter
		void SetMass(Real mass) { this->mass = mass;}

		/// resets the force accumulator
		void ResetForce() 
		{ 
			this->f = this->df;
			this->df = zeroVec; 
		}

		/// resets the displacement accumulator
		void ResetDisplacement() { this->dr = zeroVec; }

		/// resets the restitution velocity accumulator
		void ResetRestitutionVelocity() {this->vRes = zeroVec; }

		/// Corrects the predicted position by adding the accumulated displacement
		void CorrectPosition()
		{
			rPlus += dr;
		}

		/// Corrects the predicted velocity by adding the accumulated residual/restitution velocity
		void CorrectVelocity()
		{
			vPlus += vRes;
		}

		/// Adds an amount representing an external force contribution
		void AddExternalForce(const vec3<Real>& force)
		{
			this->f += force;
		}

		void AddDampingForce(const Real& b)
		{
			this->f += - b * v;
		}

		/// Synchronizes the positions and velocities at this point by copying the newly computed values into the current holders
		void SynchronizePositionsAndVelocities();

		~MassPoint(void);

		void Perturb(vec3<Real> v)
		{
			this->r += v;
		}
	};

	inline ostream& operator<<(ostream& output, const MassPoint& p) {
		output << "(" <<  p.r.x << ", " << p.r.y << ", "<< p.r.z <<")"<< " -- ";
		output << "(" <<  p.r0.x << ", " << p.r0.y << ", "<< p.r0.z <<")"<<endl;
		return output;  // for multiple << operators.
	}


