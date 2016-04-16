#include "model.h"
#include <algorithm>
#include "modelerdraw.h"
#include "FL/glut.H"
#include "force.h"
#include "mat.h"
#include "cloth.h"

using namespace std;

///////////// class Model ///////////////
Model::Model(const char* name) : properties(name) {
	ps = new ParticleSystem();
	//ps = new Cloth(Vec3f(-3.0, 4.0, 0.0), 3.0, 3.0, 3, 3);
}
GroupProperty* Model::getProperties() { return &properties; }
void Model::draw() {}
void Model::tick() {}
void Model::load() {}
ICamera* Model::getCamera() { return NULL; }
ParticleSystem* Model::getParticleSystem() { 
	return ps; 
};
// Cloth* Model::getCloth() { 
// 	return ps; 
// };
//void Model::updateForcesAndCollision(float t, Vec3f sphereLoc);


/////////////class MyModel ///////////////////
// void ground(float h);
// void head_rotation(float h);
// void body(float h);
// void left_upper_arm(float h);
// void left_lower_arm(float h, Mat4f CameraMatrix);
// void right_upper_arm(float h);
// void right_lower_arm(float h);
// void left_upper_leg(float h);
// void left_lower_leg(float h);
// void right_upper_leg(float h);
// void right_lower_leg(float h);
// void y_box(float h);
// void mass_change(float h);

MyModel::MyModel() :
  	Model("Robot"),
  	sphereCenterX("Collision Sphere Center (X)", -6.0, 6.0, 2.0, 0.1),
	sphereCenterY("Collision Sphere Center (Y)", -6.0, 6.0, -2.0, 0.1),
	sphereCenterZ("Collision Sphere Center (Z)", -6.0, 6.0, 2.0, 0.1)
  	// headRotation("Head Rotation", -90, 90, 0, 1), // minRange, maxRange, init, delta
  	// leftUpperArmTilt("Left upper arm tilt", 0, 250, 180, 1),
  	// leftLowerArmTilt("Left lower arm tilt", -115, 0, 0, 1),
  	// rightUpperArmTilt("Right upper arm tilt", 0, 250, 180, 1),
  	// rightLowerArmTilt("Right lower arm tilt", -115, 0, 0, 1),
  	// leftUpperLegTilt("Left upper leg tilt", 90, 250, 180, 1),
  	// leftLowerLegTilt("Left lower leg tilt", 0, 95, 0, 1),
  	// rightUpperLegTilt("Right upper leg tilt", 90, 250, 180, 1),
  	// rightLowerLegTilt("Right lower leg tilt", 0, 95, 0, 1),
  	// mass("Mass", 1, 10, 3, 0.5)
  {
  	 properties.add(&sphereCenterX)
  	 			.add(&sphereCenterY)
  	 			.add(&sphereCenterZ);
  	 		//.add(&headRotation)
			//   .add(&leftUpperArmTilt)
			//   .add(&leftLowerArmTilt)
			//   .add(&rightUpperArmTilt)
			//   .add(&rightLowerArmTilt)
			//   .add(&leftUpperLegTilt)
			//   .add(&leftLowerLegTilt)
			//   .add(&rightUpperLegTilt)
			//   .add(&rightLowerLegTilt)
			  //.add(&ps->restitution)
			//  .add(&mass);

  }

void MyModel::draw() {
	cloth = new Cloth(Vec3f(-3.0, 4.0, 0.0), 10.0, 10.0, 3, 3);
	// float x = sphereCenterX;
	// float y = sphereCenterY;
	// float z = sphereCenterZ;
	cloth->drawCloth(sphereCenterX, sphereCenterY, sphereCenterZ);

	/*Mat4f CameraMatrix = glGetModelViewMatrix();
	//pick up the slider values 
	float hr = headRotation.getValue();
	float lua = leftUpperArmTilt.getValue();
	float lla = leftLowerArmTilt.getValue();
	float rua = rightUpperArmTilt.getValue();
	float rla = rightLowerArmTilt.getValue();
	float lul = leftUpperLegTilt.getValue();
	float lll = leftLowerLegTilt.getValue();
	float rul = rightUpperLegTilt.getValue();
	float rll = rightLowerLegTilt.getValue();

	static GLfloat lmodel_ambient[] = {0.4,0.4,0.4,1.0};

	// define the model
	glPushMatrix();

		// Head
		glPushMatrix();
			glTranslatef(0, 3.0, 0);
			glRotatef(hr, 0, 1, 0);
			head_rotation(0.8);
		glPopMatrix();

		// Body
		glPushMatrix();
			glTranslatef(0, 1.5, 0);
			body(2.3);
		glPopMatrix();

		// Arms upper and lower, lower part is children of upper part
		glPushMatrix();
			glTranslatef(-1.2, 3.6, 0);
			glRotatef(lua, 1, 0, 0);
			left_upper_arm(1.2);
			glTranslatef(0, 1.2, 0);
			glRotatef(lla, 1, 0, 0);

			left_lower_arm(0.9, CameraMatrix);
			

		glPopMatrix();
		

		glPushMatrix();
			glTranslatef(1.2, 3.6, 0);
			glRotatef(rua, 1, 0, 0);
			right_upper_arm(1.2);

			glTranslatef(0, 1.2, 0);
			glRotatef(rla, 1, 0, 0);
			right_lower_arm(0.9);
		glPopMatrix();

		// Legs upper and lower, lower part is children of upper part
		glPushMatrix();
			glTranslatef(-0.8, 1.5, 0);
			glRotatef(lul, 1, 0, 0);
			left_upper_leg(1.2);

			glTranslatef(0, 1.2, 0);
			glRotatef(lll, 1, 0, 0);
			left_lower_leg(1.1);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.8, 1.5, 0);
			glRotatef(rul, 1, 0, 0);
			right_upper_leg(1.2);

			glTranslatef(0, 1.2, 0);
			glRotatef(rll, 1, 0, 0);
			right_lower_leg(1.1);
		glPopMatrix();

		glTranslatef(0, -0.85, 0);
		ground(0.2);

	glPopMatrix();*/
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

// void MyModel::updateForcesAndCollision(float t, Vec3f sphereLoc) {
// 	cloth->updateForcesAndCollision(t, sphereLoc);

// }

/*
void ground(float h) 
{
	setDiffuseColor(0.6,0.4,0.45);
	setAmbientColor(0.6,0.4,0.45);
	glPushMatrix();
		glScalef(30,0,30);
		y_box(h);
	glPopMatrix();

}

void head_rotation(float h) {
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );

	glPushMatrix();

			setDiffuseColor( 0.15, 0.15, 0.65 );
			setAmbientColor( 0.15, 0.15, 0.65 );

			glPushMatrix();
				glTranslatef(0, h, 0);
				glScalef(1.5, 0.7, 1.5);
				y_box(1.0f); 

				setDiffuseColor(0.75, 0.75, 0.75);
				setAmbientColor( 0.15, 0.15, 0.65 );
				drawCylinder(0.2, 0.2, 0.2);

		glPopMatrix();

	glPopMatrix();
}

void body(float h) {
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );

	glPushMatrix();
		setDiffuseColor( 0.65, 0.65, 0.65 );
		setAmbientColor( 0.65, 0.65, 0.65 );

		glPushMatrix();
			glScalef(4.0, h, 1.8);
			y_box(1.0f);
		glPopMatrix();

	glPopMatrix();
}

void left_upper_arm(float h) {
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );

	glPushMatrix();
		setDiffuseColor( 0.3, 0.3, 0.3 );
		setAmbientColor( 0.3, 0.3, 0.3 );

		glPushMatrix();
			glScalef(0.9, h, 0.9);
			y_box(1.0f);
		glPopMatrix();

	glPopMatrix();
}

void MyModel::left_lower_arm(float h, Mat4f CameraMatrix) {
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );

	glPushMatrix();
		setDiffuseColor( 0.3, 0.3, 0.3 );
		setAmbientColor( 0.3, 0.3, 0.3 );

		glPushMatrix();
			glScalef(0.4, h, 0.4);
			y_box(1.0f);
			SpawnParticles(CameraMatrix);
		glPopMatrix();

	glPopMatrix();
}

void right_upper_arm(float h) {
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );

	glPushMatrix();
		setDiffuseColor( 0.3, 0.3, 0.3 );
		setAmbientColor( 0.3, 0.3, 0.3 );

		glPushMatrix();
			glScalef(0.85, h, 1);
			y_box(1.0f);
		glPopMatrix();

	glPopMatrix();
}

void right_lower_arm(float h) {
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );

	glPushMatrix();
		setDiffuseColor( 0.3, 0.3, 0.3 );
		setAmbientColor( 0.3, 0.3, 0.3 );

		glPushMatrix();
			glScalef(0.4, h, 0.4);
			y_box(1.0f);
		glPopMatrix();

	glPopMatrix();
}

void left_upper_leg(float h) {
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );

	glPushMatrix();
		setDiffuseColor( 0.7, 0.7, 0.7 );
		setAmbientColor( 0.7, 0.7, 0.7 );

		glPushMatrix();
			
			glScalef(0.8, h, 0.8);
			y_box(1.0f);
		glPopMatrix();

	glPopMatrix();
}

void left_lower_leg(float h) {
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );

	glPushMatrix();
		setDiffuseColor( 0.7, 0.7, 0.7 );
		setAmbientColor( 0.7, 0.7, 0.7 );

		glPushMatrix();
			
			glScalef(0.4, h, 0.4);
			y_box(1.0f);
		glPopMatrix();

	glPopMatrix();
}

void right_upper_leg (float h) {
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );

	glPushMatrix();
		setDiffuseColor( 0.7, 0.7, 0.7 );
		setAmbientColor( 0.7, 0.7, 0.7 );

		glPushMatrix();
			
			glScalef(0.8, h, 0.8);
			y_box(1.0f);
		glPopMatrix();

	glPopMatrix();
}

void right_lower_leg(float h) {
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );

	glPushMatrix();
		setDiffuseColor( 0.7, 0.7, 0.7 );
		setAmbientColor( 0.7, 0.7, 0.7 );

		glPushMatrix();
			
			glScalef(0.4, h, 0.4);
			y_box(1.0f);
		glPopMatrix();

	glPopMatrix();
}

void y_box(float h) {

	glBegin( GL_QUADS );

	glNormal3d( 1.0 ,0.0, 0.0);			// +x side
	glVertex3d( 0.25,0.0, 0.25);
	glVertex3d( 0.25,0.0,-0.25);
	glVertex3d( 0.25,  h,-0.25);
	glVertex3d( 0.25,  h, 0.25);

	glNormal3d( 0.0 ,0.0, -1.0);		// -z side
	glVertex3d( 0.25,0.0,-0.25);
	glVertex3d(-0.25,0.0,-0.25);
	glVertex3d(-0.25,  h,-0.25);
	glVertex3d( 0.25,  h,-0.25);

	glNormal3d(-1.0, 0.0, 0.0);			// -x side
	glVertex3d(-0.25,0.0,-0.25);
	glVertex3d(-0.25,0.0, 0.25);
	glVertex3d(-0.25,  h, 0.25);
	glVertex3d(-0.25,  h,-0.25);

	glNormal3d( 0.0, 0.0, 1.0);			// +z side
	glVertex3d(-0.25,0.0, 0.25);
	glVertex3d( 0.25,0.0, 0.25);
	glVertex3d( 0.25,  h, 0.25);
	glVertex3d(-0.25,  h, 0.25);

	glNormal3d( 0.0, 1.0, 0.0);			// top (+y)
	glVertex3d( 0.25,  h, 0.25);
	glVertex3d( 0.25,  h,-0.25);
	glVertex3d(-0.25,  h,-0.25);
	glVertex3d(-0.25,  h, 0.25);

	glNormal3d( 0.0,-1.0, 0.0);			// bottom (-y)
	glVertex3d( 0.25,0.0, 0.25);
	glVertex3d(-0.25,0.0, 0.25);
	glVertex3d(-0.25,0.0,-0.25);
	glVertex3d( 0.25,0.0,-0.25);

	glEnd();
}
*/
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


