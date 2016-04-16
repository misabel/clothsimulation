#ifndef __CLOTHPARTICLE_H__
#define __CLOTHPARTICLE_H__

#include "vec.h"
#include <vector>
#include "force.h"
#include "modelerdraw.h"

using namespace std;

class ClothParticle {
public:
	ClothParticle(bool fixed);
	ClothParticle(float Mass, const Vec3f Position, const Vec3f Velocity, bool fixed);
	ClothParticle(const Vec3f Position, const Vec3f Velocity, bool fixed);
	ClothParticle(const Vec3f Position, bool fixed);

	float getMass() { return _mass; }
	Vec3f getPosition() { return _position; }
	Vec3f getVelocity() { return _velocity; }
	void setMass(float m) { _mass = m;}
	void setPosition(Vec3f p);
	void setVelocity(Vec3f v);
	void updatePosition(Vec3f p) {
		_position = _position + p;
	} 
	Vec3f getNormal() {
		return _accNormal;
	}
	void clearNormal() {

		_accNormal = Vec3f(0.0);
	}
	void addNormal (Vec3f n){

		_accNormal = _accNormal + n;
	}


	
	// void addForce(Vec3f f);
	// Vec3f* getState();
	// void setState(Vec3f p, Vec3f v);
	void derivEval(float deltaT, Vec3f forces);

protected:
	float _mass;
	Vec3f _position;
	Vec3f _prevPosition;
	Vec3f _velocity;
	Vec3f _acceleration;
	Vec3f _accNormal;
	// Vec3f _forces;
	Vec3f* _state;
	bool _isFixed;

private:
	// void updateState();
};

#endif __CLOTHPARTICLE_H__