#include "clothParticle.h"


ClothParticle::ClothParticle(bool fixed){
	_mass = 0.5;
	_position = Vec3f(0.0);
	_velocity = Vec3f(0.0);
	_isFixed = fixed;

	// _state = new Vec3f[2];
	// _forces = Vec3f(0.0);
	// updateState();
}
ClothParticle::ClothParticle(float mass, const Vec3f Position, const Vec3f Velocity, bool fixed){
	_mass = mass;
	_position = Vec3f(Position);
	_velocity = Velocity;
	_isFixed = fixed;
	// _state = new Vec3f[2];
	// _forces = Vec3f(0.0);
	// updateState();
}
ClothParticle::ClothParticle(const Vec3f Position, const Vec3f Velocity, bool fixed) 
{
	_mass = 1.0;
	_position = Vec3f(Position);
	_velocity = Velocity;
	_isFixed = fixed;
	// _state = new Vec3f[2];
	// _forces = Vec3f(0.0);
	// updateState();
}
ClothParticle::ClothParticle(const Vec3f Position, bool fixed) 
{
	_mass = 1.0;
	_position = Vec3f(Position);
	_velocity = Vec3f(0.0);
	_isFixed = fixed;
	// _state = new Vec3f[2];
	// _forces = Vec3f(0.0);
	// updateState();

}

// void ClothParticle::addForce(Vec3f force)
// {
// 	_forces += force;

// }

// Vec3f* ClothParticle::getState()
// {
// 	return _state;
// }

// void ClothParticle::setState(Vec3f p, Vec3f v)
// {
// 	_position = p;
// 	// _position[0] = 0.0;
// 	// _position[2] = 0.0;
// 	_velocity = v;
// 	updateState();
// }

// void ClothParticle::updateState()
// {
// 	_state[0] = _position;
// 	_state[1] = _velocity;

// 	// cout << "velocity is: " << _velocity[0] << ", " << _velocity[1] << ", " << _velocity[2] << endl;
// }

void ClothParticle::setPosition(Vec3f p)
{
	_position = p;
	// updateState();
}

void ClothParticle::setVelocity(Vec3f v)
{
	_velocity = v;
	// updateState();
}

void ClothParticle::derivEval(float deltaT, Vec3f forces)
{
	Vec3f new_position = _velocity * deltaT;
	Vec3f new_velocity = forces/_mass * deltaT;

	_position += new_position;
	_velocity += new_velocity;
	// Vec3f derivative[2] = {_velocity, (_forces/_mass)};
	// cout << _velocity[0] << ", " << _velocity[1] << ", " << _velocity[2] << endl;
	// _forces = Vec3f(0.0);
	// cout << _forces[1] << endl;
	// return derivative;
}

