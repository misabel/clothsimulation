#ifndef __CLOTH_H__
#define __CLOTH_H__

#include "vec.h"
#include <vector>
#include "force.h"
#include "clothParticle.h"
#include "constraint.h"
#include "properties.h"

class Cloth {
private: 
		int x_num; // number of particle in x-asix
		int y_num;
		std::vector<ClothParticle> cpList;
		std::vector<Constraint> cList;
		Vec3f ballLoc = Vec3f(2.0f, -2.0f, 2.0f); // get init in model.cpp

		//RangeProperty sphereCenterX, sphereCenterY, sphereCenterZ;

public:
	/** Constructor **/
	Cloth(Vec3f origin, float width, float height, int x, int y);

	/** Destructor **/
	virtual ~Cloth();

	ClothParticle* getClothParticle(int x, int y) {

		return &cpList[y * x_num + x];
	}

	Vec3f calculateNormal(Vec3f p1, Vec3f p2, Vec3f p3);

	/** Simulation fxns **/
	// This fxn should render all particles in the system,
	// at current time t.
	virtual void drawCloth(RangeProperty sphereCenterX, RangeProperty sphereCenterY, RangeProperty sphereCenterZ);

	// This function should compute forces acting on all particles
	// and update their state (pos and vel) appropriately.
	 virtual void updateForcesAndCollision(float t, Vec3f sphereLoc);

	 virtual void drawFan();

	 virtual void bakeParticles(float t);

	// // This function should reset the system to its initial state.
	// // When you need to reset your simulation, PLEASE USE THIS FXN.
	// // It sets some state variables that the UI requires to properly
	// // update the display.  Ditto for the following two functions.
	 virtual void resetSimulation(float t);

	// // This function should start the simulation
	 virtual void startSimulation(float t);

	// // This function should stop the simulation
	 virtual void stopSimulation(float t);

	 	// This function should clear out your data structure
	// of baked particles (without leaking memory).
	virtual void clearBaked();	

	Vec3f findNormal(Vec3f p1, Vec3f p2, Vec3f p3);

	void clearAllNormals() {

		for(int i = 0; i < cpList.size(); i++){

			cpList[i].clearNormal();
		}
	}


	// These accessor fxns are implemented for you
	float getBakeStartTime() { return bake_start_time; }
	float getBakeEndTime() { return bake_end_time; }
	float getBakeFps() { return bake_fps; }
	bool isSimulate() { return simulate; }
	bool isDirty() { return dirty; }
	void setDirty(bool d) { dirty = d; }

	float bake_fps;						// frame rate at which simulation was baked
	float bake_start_time;				// time at which baking started 
										// These 2 variables are used by the UI for
										// updating the grey indicator 
	float bake_end_time;


protected:

	/** General state variables **/
	bool simulate;						// flag for simulation mode
	bool dirty;							// flag for updating ui (don't worry about this)
};


#endif __CLOTH_H__