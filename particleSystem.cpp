#pragma warning(disable : 4786)

#include "particleSystem.h"
#include "modelerui.h"
#include "force.h"
#include "particle.h"


#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>
//#include <limits>

using namespace std;

static float prevT;

static Force Forces;

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() : restitution("Restitution", 0.0f, 2.0f, 1.0f, 0.1f)
{
	//Gravity G = new Gravity();

	bake_fps = 0;
	bake_start_time = 0;
	bake_end_time = 0;



	// Leave these here; the UI needs them to work correctly.
	dirty = false;
	simulate = false;
	spawnPoint = Vec3f(30.0);
}


/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{

}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{

	prevT = t;
	bake_start_time = 0;

	// These values are used by the UI ...
	// negative bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
	bake_start_time = 0;

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	if (simulate){
		const float deltaT = t - prevT;
		prevT = t;
		float k = 0.001;

		for (auto &particle : _Particles) {
			Vec3f force = Vec3f(0.0f, 0.0f, 0.0f);
			Vec3f position = particle->getPosition();
			Vec3f velocity = particle->getVelocity();
			float mass = particle->getMass();


			force += mass * Force::G();
			force -= Force::K() * velocity;
			Force::randomizeK();

			particle->derivEval(deltaT, force);

			// Plane Collision detection 
			Vec3f N = Vec3f(0, 1, 0);
			if(position[1] < 0 && velocity * N < 0) {
				Vec3f vn;
				Vec3f vt;
				vn = (N * velocity) * N;
				vt = velocity-vn;
				velocity = vt - k * vn;

				particle->setVelocity(Vec3f(velocity[0], -velocity[1], velocity[2]));
			}

				/*Vec3f N = Vec3f(0, 1, 1);
				if(i)*/

			// Vec3f new_position = velocity;
			// Vec3f new_velocity = force / mass;

			// new_position *= deltaT;
			// new_velocity *= deltaT;

			// new_position +=position;
			// new_velocity +=velocity;



			// particle->setPosition(new_position);
			// particle->setVelocity(new_velocity);
			// cout << "generating particle" << endl;

			// for(auto &f : _Forces){
			// 	force += f->getForce();
			// }
			// particle->addForce(particle->getMass() * Force::G());

			// particle->addForce(-(Force::K() * particle->getVelocity()));

			// Vec3f* oldState = particle->getState();
			// cout << "state is " << oldState[0][0] << ", " << oldState[0][1] << ", " << oldState[0][2] << endl;
			// Vec3f* newState = particle->derivEval();
			// newState[0] *=deltaT;
			// newState[1] *=deltaT;
			// newState[0] += oldState[0];
			// newState[1] += oldState[1];

			// particle->setState(newState[0], newState[1]);
			// cout << particle->getState()[0] << endl;
			// velocity += force / particle->getMass() * deltaT;
			// position += velocity * deltaT;
			// particle->setVelocity(velocity);
			// particle->setPosition(position);
		}
	
	}
	

	// Debugging info
	/*if( t - prevT > .08 )
		printf("(!!) Dropped Frame %lf (!!)\n", t-prevT);*/
	//prevT = t;
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	// YOUR CODE HERE
	if (simulate) {
		// for (vector<Particle*>::iterator it = particles[t].begin(); it != particles[t].end(); it++) {
		// 	(*it)->Draw();
		// }
		for (auto &particle : _Particles) {
			particle->Draw();
			// cout << "drawing" << endl;
		}
	}
}


/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	// TODO (baking is extra credit)

}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	// TODO (baking is extra credit)

}

void ParticleSystem::addParticleStartingAt(Vec4f WorldPoint){
	// cout << "spawn point is: "<< WorldPoint[0] << ", " << WorldPoint[1] << ", " << WorldPoint[2] << endl;
	spawnPoint = Vec3f(WorldPoint);
	cout << "spawning at "<< spawnPoint[0] << ", " << spawnPoint[1] << ", " << spawnPoint[2] << endl;
}



