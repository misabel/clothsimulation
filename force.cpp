#include "force.h"

Vec3f Force::g = Vec3f(0.0, -9.81, 0.0);
float Force::k = 0.1;

void Force::randomizeK()
{
	 k = float(rand() % 5 + 0) / 10.0;
}