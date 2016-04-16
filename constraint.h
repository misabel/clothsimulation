#ifndef __CONSTRAINT_H__
#define __CONSTRAINT_H__

#include "vec.h"
#include <vector>
#include "clothParticle.h"
using namespace std;

class Constraint {
public:
	/** Constructor **/
	Constraint(ClothParticle p1, ClothParticle p2);

	/** Destructor **/
	virtual ~Constraint();

	/** Simulation fxns **/
	// This fxn should render all particles in the system,
	// at current time t.
	virtual void drawConstraint();//ClothParticle p1, ClothParticle p2);

	virtual void satisfyConstraints();

	virtual ClothParticle getP1() {
		 return _p1;
	}

	virtual ClothParticle getP2() {
		return _p2;
	}

protected:
	// ClothParticle *p1;
	// ClothParticle *p2;
	ClothParticle _p1;
	ClothParticle _p2;

private: 
	float restLength; // init distance btw two ClothParticles. 

};


#endif __CONSTRAINT_H__