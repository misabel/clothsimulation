#include "particle.h"


Particle::Particle(){
	_mass = 0.5;
	_position = Vec3f(0.0);
	_velocity = Vec3f(0.0);
	// _state = new Vec3f[2];
	// _forces = Vec3f(0.0);
	// updateState();
}
Particle::Particle(float mass, const Vec3f Position, const Vec3f Velocity){
	_mass = mass;
	_position = Vec3f(Position);
	_velocity = Velocity;
	// _state = new Vec3f[2];
	// _forces = Vec3f(0.0);
	// updateState();
}
Particle::Particle(const Vec3f Position, const Vec3f Velocity) 
{
	_mass = 1.0;
	_position = Vec3f(Position);
	_velocity = Velocity;
	// _state = new Vec3f[2];
	// _forces = Vec3f(0.0);
	// updateState();
}
Particle::Particle(const Vec3f Position) 
{
	_mass = 1.0;
	_position = Vec3f(Position);
	_velocity = Vec3f(0.0);
	// _state = new Vec3f[2];
	// _forces = Vec3f(0.0);
	// updateState();

}

// void Particle::addForce(Vec3f force)
// {
// 	_forces += force;

// }

// Vec3f* Particle::getState()
// {
// 	return _state;
// }

// void Particle::setState(Vec3f p, Vec3f v)
// {
// 	_position = p;
// 	// _position[0] = 0.0;
// 	// _position[2] = 0.0;
// 	_velocity = v;
// 	updateState();
// }

// void Particle::updateState()
// {
// 	_state[0] = _position;
// 	_state[1] = _velocity;

// 	// cout << "velocity is: " << _velocity[0] << ", " << _velocity[1] << ", " << _velocity[2] << endl;
// }

void Particle::setPosition(Vec3f p)
{
	_position = p;
	// updateState();
}

void Particle::setVelocity(Vec3f v)
{
	_velocity = v;
	// updateState();
}

void Particle::derivEval(float deltaT, Vec3f forces)
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

void Particle::Draw()
{
	// glPushMatrix();

		glPushMatrix();
			glTranslatef(_position[0], _position[1], _position[2]);
			drawSphere(.10);
		glPopMatrix();

		// setDiffuseColor(0.65,0.45,0.2);
		// setAmbientColor(0.65,0.45,0.2);
		// glPushMatrix();
		// 	glScalef(30,0,30);
		// 	y_box(1);
		// glPopMatrix();

	// glPopMatrix();
}