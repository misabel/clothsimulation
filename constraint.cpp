#include "cloth.h"
#include <FL/gl.h>
#include <math.h>
#include "constraint.h"
/*
Constraint::Constraint(ClothParticle p1, ClothParticle p2) {
	Vec3f v = p1.getPosition() - p2.getPosition();
	restLength = v.length();
}

Constraint::~Constraint() {

}

void Constraint::drawConstraint() {


}

void Constraint::satisfyConstraints() {

	Vec3f v12 = p2->getPosition() - p1->getPosition();
	float curDist = v12.length();
	Vec3f corVecHalf = v12 * (1 - restLength/curDist) * 0.5;
	p1->updatePosition(corVecHalf);
	p2->updatePosition(-corVecHalf);

}
*/