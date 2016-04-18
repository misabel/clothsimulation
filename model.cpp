#include "model.h"
#include <algorithm>
#include "modelerdraw.h"
#include "FL/glut.H"
#include "force.h"
#include "mat.h"
#include "cloth.h"

using namespace std;

///////////// class Model ///////////////
Model::Model(const char* name) : properties(name),
sphereCenterX1("Cloth Sphere Center (X)", -10.0, 20.0, 2.0, 0.1),
	sphereCenterY1("Cloth Sphere Center (Y)", -10.0, 20.0, -2.0, 0.1),
	sphereCenterZ1("Cloth Sphere Center (Z)", -10.0, 20.0, 2.0, 0.1) {

		properties.add(&sphereCenterX1)
  	 			.add(&sphereCenterY1)
  	 			.add(&sphereCenterZ1);
	ps = new ParticleSystem();
	cloth = new Cloth();
}
GroupProperty* Model::getProperties() { return &properties; }
void Model::draw() {
	cloth->drawCloth(sphereCenterX1.getValue(), sphereCenterY1.getValue(), sphereCenterZ1.getValue());
}
void Model::tick() {}
void Model::load() {}
ICamera* Model::getCamera() { return NULL; }
ParticleSystem* Model::getParticleSystem() { 
	return ps; 
};

Cloth* Model::getCloth() {
	return cloth;
};


MyModel::MyModel() :
  	Model("Cloth"),
  	
	sphereCenterX("Collision Sphere Center (X)", -6.0, 6.0, 2.0, 0.1),
	sphereCenterY("Collision Sphere Center (Y)", -6.0, 6.0, -2.0, 0.1),
	sphereCenterZ("Collision Sphere Center (Z)", -6.0, 6.0, 2.0, 0.1)
  {
  	 // properties.add(&sphereCenterX)
  	 // 			.add(&sphereCenterY)
  	 // 			.add(&sphereCenterZ);
  		// 		.add(&width)
  	 // 			.add(&length)
  	 // 			.add(&density);



  	 		



  }

void MyModel::draw() {

	// cloth->drawCloth(sphereCenterX.getValue(), sphereCenterY.getValue(), sphereCenterZ.getValue());
	// sphereCenterX1.setValue(sphereCenterX.getValue());
 //  	sphereCenterY1.setValue(sphereCenterY.getValue());
 //  	sphereCenterZ1.setValue(sphereCenterZ.getValue());
	// delete cloth;
	// cloth = new Cloth(Vec3f(-2.0, 3.5, .3), width.getValue(), length.getValue(), density.getValue(), density.getValue());
	// float x = sphereCenterX;
	// float y = sphereCenterY;
	// float z = sphereCenterZ;
	// cloth->updateForcesAndCollision(0.25);
	// cloth->drawCloth(sphereCenterX.getValue(), sphereCenterY.getValue(), sphereCenterZ.getValue());

	
}

Mat4f MyModel::glGetModelViewMatrix()
{
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	Mat4f matMV = Mat4f(m[0], m[1], m[2], m[3],
            m[4], m[5], m[6], m[7],
            m[8], m[9], m[10], m[11],
            m[12], m[13], m[14], m[15] );

	return matMV.transpose();
}

void MyModel::SpawnParticles(Mat4f CameraTransforms)
{
	Mat4f WorldMatrix = CameraTransforms.inverse() * (CameraTransforms * glGetModelViewMatrix());

	Vec4f WorldPoint = WorldMatrix * Vec4f(0.0, 0.0, 0.0, 1.0) + Vec4f(30.0, 30.0, 30.0, 30.0);
	//cout << "spawning at "<< spawnPoint[0] << ", " << spawnPoint[1] << ", " << spawnPoint[2] << endl;

	//ps->addParticleStartingAt(WorldPoint);

}

///////////// class Light ///////////////
Light::Light(const char* name, GLuint lightNumber,
						 float x, float y, float z,
						 float dr, float dg, float db,
						 float sr, float sg, float sb,
						 float ar, float ag, float ab) :
	Model(name),
    lightX("Light X", -10, 10, x, .1f),
	lightY("Light Y", -10, 10, y, .1f),
	lightZ("Light Z", -10, 10, z, .1f),
	diffuse("Diffuse", dr, dg, db),
	specular("Specular", sr, sg, sb),
	ambient("Ambient",  ar, ag, ab),
	isOn("This light is on", true)
	{

	properties.add(&isOn)
			  .add(&diffuse)
			  .add(&specular)
			  .add(&ambient)
			  .add(&lightX)
			  .add(&lightY)
			  .add(&lightZ);
	
	properties.setCollapsed(true);

	this->lightNumber = lightNumber;
}

void Light::draw(bool directional) {
	if (!isOn.getValue()) {
		// Set values to 0
		const float zeroes[4] = {0,0,0,0};
		glLightfv( lightNumber, GL_DIFFUSE, zeroes );
		glLightfv( lightNumber, GL_SPECULAR, zeroes );
		glLightfv( lightNumber, GL_AMBIENT, zeroes );
		return;
	}

	// Enable OpenGL lighting
	// TODO: check to see if it's already on
	glEnable( GL_LIGHTING );

	// Enable the light we're using
	// TODO: check to see if it's already on
	glEnable(lightNumber);

	// Create array containing light position
	GLfloat lightPositionArray[4] = {
		lightX.getValue(),
		lightY.getValue(),
		lightZ.getValue(),
		directional ? 0.f : 1.f
	};

	// Return if the light is directional but has no direction.
	if (directional && lightPositionArray[0] == 0
		&& lightPositionArray[1] == 0
		&& lightPositionArray[2] == 0) return;

	// Create array containing light diffuse color
	GLfloat lightDiffuseArray[4] = {
		diffuse.getRed(),
		diffuse.getGreen(),
		diffuse.getBlue(),
		1
	};

	// Create array for light's specular color
	GLfloat lightSpecularArray[4] = {
		specular.getRed(),
		specular.getGreen(),
		specular.getBlue(),
		1
	};

	// Create array for light's ambient color
	GLfloat lightAmbientArray[4] = {
		ambient.getRed(),
		ambient.getGreen(),
		ambient.getBlue(),
		1
	};

	// Send light to OpenGL
	glLightfv( lightNumber, GL_POSITION, lightPositionArray );
	glLightfv( lightNumber, GL_DIFFUSE, lightDiffuseArray );
	glLightfv( lightNumber, GL_SPECULAR, lightSpecularArray );
	glLightfv( lightNumber, GL_AMBIENT, lightAmbientArray );

	// Draw a marker for the light if allowed
	if (ModelerDrawState::Instance()->showMarkers) {
		const float lightRadius = .5f;
		const float directionalOrbit = 5.f;
		
		glPushMatrix();
			glTranslatef(lightPositionArray[0],
						 lightPositionArray[1],
						 lightPositionArray[2]);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glColor3f(1, 1, 1);
			glBegin(GL_LINES);
				glVertex3f(-lightRadius, 0, 0);
				glVertex3f(lightRadius, 0, 0);
				glVertex3f(0, -lightRadius, 0);
				glVertex3f(0, lightRadius, 0);
				glVertex3f(0, 0, -lightRadius);
				glVertex3f(0, 0, lightRadius);
			glEnd();
			glEnable(GL_LIGHTING);
		glPopMatrix();
	}
}

PointLight::PointLight(const char* name, GLuint lightNumber,
				float x, float y, float z,
				float dr, float dg, float db,
				float sr, float sg, float sb,
				float ar, float ag, float ab,
				float attA, float attB, float attC) :
	Light(name, lightNumber, x, y, z, dr, dg, db, sr, sg, sb, ar, ag, ab),
	attenA("Quadratic Attenuation ^ 5", 0, 1, attA, .05f),
	attenB("Linear Attenuation ^ 5", 0, 1, attB, .05f),
	attenC("Constant Attenuation ^ 5", 0, 1, attC, .05f)
{
	properties.add(&attenA).add(&attenB).add(&attenC);
}

void PointLight::draw() {
	// Set attenuation
	glLightf(lightNumber, GL_CONSTANT_ATTENUATION, pow(attenC.getValue(), 5));
	glLightf(lightNumber, GL_LINEAR_ATTENUATION, pow(attenB.getValue(), 5));
	glLightf(lightNumber, GL_QUADRATIC_ATTENUATION, pow(attenA.getValue(), 5));

	Light::draw(false);
}

/**
 * A directional light.
 */
DirectionalLight::DirectionalLight(const char* name, GLuint lightNumber,
				float x, float y, float z,
				float dr, float dg, float db,
				float sr, float sg, float sb,
				float ar, float ag, float ab) :
	Light(name, lightNumber, x, y, z, dr, dg, db, sr, sg, sb, ar, ag, ab)
{
}

void DirectionalLight::draw() {
	Light::draw(true);
}

/**
 * A spotlight light.
 */
SpotLight::SpotLight(const char* name, GLuint lightNumber,
				float x, float y, float z,
				float dr, float dg, float db,
				float sr, float sg, float sb,
				float ar, float ag, float ab,
				float attA, float attB, float attC,
				float sX, float sY, float sZ, float beta, float alpha) :
	Light(name, lightNumber, x, y, z, dr, dg, db, sr, sg, sb, ar, ag, ab),
	attenA("Quadratic Attenuation ^ 5", 0, 1, attA, .05f),
	attenB("Linear Attenuation ^ 5", 0, 1, attB, .05f),
	attenC("Constant Attenuation ^ 5", 0, 1, attC, .05f),
	spotX("Spotlight X", -10, 10, sX, .01f),
	spotY("Spotlight Y", -10, 10, sY, .01f),
	spotZ("Spotlight Z", -10, 10, sZ, .01f),
	cutoff("Cutoff", 0, 90, alpha, .01f),
	falloff("Angular Falloff", 0, 10, beta, .01f)
{
	properties.add(&attenA).add(&attenB).add(&attenC).add(&spotX)
				.add(&spotY).add(&spotZ).add(&cutoff).add(&falloff);
}

void SpotLight::draw() {
	// Set attenuation
	glLightf(lightNumber, GL_CONSTANT_ATTENUATION, pow(attenC.getValue(), 5));
	glLightf(lightNumber, GL_LINEAR_ATTENUATION, pow(attenB.getValue(), 5));
	glLightf(lightNumber, GL_QUADRATIC_ATTENUATION, pow(attenA.getValue(), 5));

	GLfloat light_direction[] = { 
		(GLfloat)spotX.getValue(), 
		(GLfloat)spotY.getValue(), 
		(GLfloat)spotZ.getValue() 
	};
	glLightfv(lightNumber, GL_SPOT_DIRECTION, light_direction);
	
	glLightf(lightNumber, GL_SPOT_CUTOFF, cutoff.getValue());
	glLightf(lightNumber, GL_SPOT_EXPONENT, falloff.getValue());
	Light::draw(false);
}


