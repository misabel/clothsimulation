#include "cloth.h"
#include <FL/gl.h>
#include <math.h>
#include "constraint.h"

Constraint::Constraint(ClothParticle p1, ClothParticle p2) : _p1(p1), _p2(p2) {
	_p1 = p1;
	_p2 = p2;
	Vec3f v = _p1.getPosition() - _p2.getPosition();
	restLength = v.length();
}

Constraint::~Constraint() {

}

void Constraint::drawConstraint() {
	Vec3f tmpP1 = _p1.getPosition();
	Vec3f tmpP2 = _p2.getPosition();
	glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(tmpP1[0],tmpP1[1], tmpP1[2]);
		glVertex3f(tmpP2[0],tmpP2[1], tmpP2[2]);
	glEnd();
}

void Constraint::satisfyConstraints() {
	Vec3f v12 = _p2.getPosition() - _p1.getPosition();
	float curDist = v12.length();
	Vec3f corVecHalf = v12 * (1 - restLength/curDist) * 0.5;
	_p1.updatePosition(corVecHalf);
	_p2.updatePosition(-corVecHalf);
}
