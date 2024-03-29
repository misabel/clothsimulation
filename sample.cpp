
// The sample box model.  You should build a file
// very similar to this for when you make your model in
// order to plug in to the animator project.

#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning (disable : 4786)
#pragma warning (disable : 4312)


#include "modelerview.h"
#include "modelerdraw.h"
#include "modelerui.h"
#include "vault.h"
#include "FL/glut.H"
#include "model.h"
#include "cameras.h"
#include <cmath>
#include <time.h>

/** The scene, which includes the lights and models. */
class Scene : public Model {
protected:
///////////////////////////////// TEXTURES ////////////////////////////////////
	Texture2D texture;

///////////////////////////////// SHADERS /////////////////////////////////////
	ShaderProgram shader;
	ShaderProgram toonShader;
	ShaderProgram schlickShader;
	ShaderProgram textShader;
	ShaderProgram spotLightShader;

//////////////////////////////// PROPERTIES ///////////////////////////////////
	// Switches for spheres
	BooleanProperty useTexture;
	// Switch for showing/hiding reference unit sphere
	BooleanProperty showReferenceUnitSphere;

	// Lets you pick what shapes to use for the default model!
	ChoiceProperty shapeChoice;
	ChoiceProperty shaderChoice;

	BooleanProperty useShader;
	BooleanProperty useToonShader;
	BooleanProperty useSchlickShader;
	BooleanProperty useTextShader;
	BooleanProperty useSpotLightShader;

	// Some slider properties
	RangeProperty rotateX, rotateY;
	RangeProperty brightness;
	RangeProperty N1;
	RangeProperty N2;

	// Diffuse color picker
	RGBProperty diffuse;

	// Scene lights
	PointLight pointLight;
	DirectionalLight directionalLight;
	SpotLight spotLight;

	// A robot arm model
	//RobotArm robotArm;
	MyModel myModel;

public:
	ParticleSystem ps;
	 Cloth cloth;

	/** Modeler calls this to get our particle system. */
	ParticleSystem* getParticleSystem() { 
		return &ps; 
	}

	// 	Cloth* getCloth() { 
	// 	return &cloth; 
	// }

	
	RangeProperty sphereCenterX, sphereCenterY, sphereCenterZ;
	

	/** Construct the scene */
	Scene() :
	    // You have to call the parent class's constructor, tso provide a
	    // name for the model.
	    Model("Scene"),

		// Construct textures and shaders. 
		// They won't be loaded until the model is drawn for the first time.
		texture("checkers.png"),
		shader("shader.vert", "shader.frag", NULL),

		toonShader("toonShader.vert", "toonShader.frag", NULL),
		schlickShader("schlickShader.vert", "schlickShader.frag", NULL),
		textShader("textShader.vert", "textShader.frag", NULL),
		spotLightShader("spotLightShader.vert", "spotLightShader.frag", NULL),

		// Call the constructors for the lights
		spotLight("Spot Light", GL_LIGHT2, /**direction part**/ -5, 5, 5, /**diffuse part**/ 1.0, 0.0, 0.9, 
		/**specular part**/ 1.0, 0.5, 0.5, /**ambient part**/ .2f, 0.1, 0.1 /**attenuation part**/, 0.65, 0.33, 0.38,
		/**spotX/Y/Z part**/ -7, 10, 8, /**spotlight angle**/ 5, 30 ),
		pointLight("Point Light", GL_LIGHT1, /**direction part**/ 0.54, -0.54, -3.29, /**diffuse part**/ 0.666, 0.940, 1.0, 
		/**specular part**/ 1.0, 0.409, 0.550, /**ambient part**/ .740f, 0.246, 0.290 /**attenuation part**/, 0.4, 0.7, 0),
		directionalLight("Directional Light", GL_LIGHT0, /**direction part**/ 5, 5, 5, /**diffuse part**/ 0.5f, 0.071f, 0.172f, 
		/**specular part**/ 0.289f, 0.75f, 0.0f )

		// Now, call the constructors for each Property:
		, useTexture("Use Checkered Texture", true),
		showReferenceUnitSphere("Show Reference Unit Sphere", false),
		shapeChoice("Model Shape:", "Sphere|Cube|Cylinder|Torus|Icosahedron|Teapot|Revolution|My Model", 0), //557 animator UI allows shapes + Model
		shaderChoice("Shape To Use:", "None|Cartoon Shader|Schlick Shader|Texture Blinn Phong Shader|Spotlight Shader", 0),
		useShader("Use My Shader", true),
		useToonShader("Use Cartoon Shader", false),
		useSchlickShader("Use Schlick Shader", false),
		useTextShader("Use Texture Blinn Phong Shader", false),
		useSpotLightShader("Use Spot Light Shader", false),
		rotateX("Rotate Basic Shape X", -180, 180, 0, 1),
		rotateY("Rotate Basic Shape Y", -180, 180, 0, 1),
		brightness("Brightness", 0.0f, 1.0f, 1.0f, 0.1f),
		N1("Schlick N1", 0.0f, 1.0f, 1.0f, 0.1f),
		N2("Schlick N2", 0.0f, 1.0f, 1.0f, 0.1f),
		diffuse("Diffuse Color", 1.0, 0.7, .4)
		,
		sphereCenterX("Collision Sphere Center (X)", -6.0, 6.0, 2.0, 0.1),
		sphereCenterY("Collision Sphere Center (Y)", -6.0, 6.0, -2.0, 0.1),
		sphereCenterZ("Collision Sphere Center (Z)", -6.0, 6.0, 2.0, 0.1)
	{
		// If you have child Models, like the MobileLight model from model.h,
		// you can add their property groups, and they will appear in the list
		// in the top left corner of Modeler, under this model's entry:
		properties.add(pointLight.getProperties())
				  .add(directionalLight.getProperties())
				  .add(spotLight.getProperties())
				  .add(myModel.getProperties())
				  ;
		//properties.add(robotArm.getProperties());

		// Finally, add all the properties to this model's PropertyGroup.
		properties.add(&useTexture)
				  ;
		properties.add(&showReferenceUnitSphere)
				  .add(&shapeChoice)
				  .add(&shaderChoice);
		properties.add(&rotateX)
				.add(&rotateY)
				.add(&useShader)
				.add(&useToonShader)
				.add(&useSchlickShader)
				.add(&useTextShader)
				.add(&useSpotLightShader)
				;
		properties.add(&sphereCenterX)
				.add(&sphereCenterY)
				.add(&sphereCenterZ);
		properties.add(&brightness)
				  .add(&N1)
				  .add(&N2)
				  .add(&diffuse)
				  ;

				  
	}



	/**
	 * Modeler calls this method once an OpenGL context becomes available,
	 * (which happens before the first time draw() is called), and whenever
	 * you click the "Reload Textures and Shaders" menu option.
	 *
	 * Make sure you call the load() methods of each of your textures and
	 * shaders here.
	 */
	void load() {
		texture.load();
		shader.load();
		toonShader.load();
		schlickShader.load();
		textShader.load();
		spotLightShader.load();
	}

	/**
	 * Modeler calls this method many times a second when the Animate
	 * checkbox is checked.
	 */
	void tick() {
		// You can put code here to increment animation counters for
		// extra credit.
	}
	
	/** Draw the model. */
	void drawModel() {

		int divisions=16; 		
        switch(ModelerDrawState::Instance()->m_quality)
        {
        case HIGH: 
			divisions = 64; break;
		case MEDIUM: 
			divisions = 20; break;
		case LOW:
			divisions = 12; break;
		case POOR:
			divisions = 8; break;
        }

		// Remember our current modelview matrix, which is in world space.
		glPushMatrix();

		    // Rotate around the X-axis
			glRotatef(rotateX.getValue(), 1, 0, 0);

			// Rotate around the Y-axis
			glRotatef(rotateY.getValue(), 0, 1, 0);


			// Draw some object at the origin.
			switch (shapeChoice.getValue()) {
			case 0: // sphere
				// glRotatef(-90.f,1.f,0.f,0.f);
				// glutSolidSphere(1.0, divisions, divisions);
				// glRotatef(90.f,1.f,0.f,0.f);
				break;
			case 1: // box
				glRotatef(-45.f,0.f,1.f,0.f);
				glutSolidCube(2);
				glRotatef(45.f,0.f,1.f,0.f);
				break;
			case 2: // cylinder
				glRotatef(90.f,1.f,0.f,0.f);
				drawCylinder(1.8,1.0,1.8);
				glRotatef(-90.f,1.f,0.f,0.f);
				break;
			case 3: // torus
				glutSolidTorus(.5,1,divisions,divisions);
				break;
			case 4: // icosahedron
				glutSolidIcosahedron();
				break;
			case 5: // teapot
				glutSolidTeapot(1);
				break;
			case 6:
				drawRevolution(1);
				break;
			case 7:
				// Draw the default sphere for collision
				glPopMatrix();
				// draw();
				myModel.draw();

				break;
				
			}

			// if (showReferenceUnitSphere.getValue() ) {
			//     // Now go 1 units in the +y direction,
			// 	// and 1 units in the NEGATIVE x direction.
			// 	glTranslated(-2, -1, 0);
			//     // Now go 1 units in the +y direction,
			// 	// and 1 units in the NEGATIVE x direction.
			// 		glRotatef(-90.f,1.f,0.f,0.f);
			// 		drawSphere(.5);
			// 		glRotatef(90.f,1.f,0.f,0.f);
			// 	glPopMatrix();
			// }
			
			// Go back to "world space"
			glPopMatrix();

		// if (shapeChoice.getValue() != 7)
		// {

		// 	// Draw the floor.  Here's a basic example of texture mapping.
		// 	glBegin(GL_QUADS);
		// 	glTexCoord2f(0, 1); // specify the texture coordinate
		// 	glNormal3f(0, 1, 0); // specify the surface's normal at this vertex
		// 	glVertex3f(-10, -2, -10); // both before its corresponding vertex

		// 	glTexCoord2f(1, 1);
		// 	glNormal3f(0, 1, 0);
		// 	glVertex3f(10, -2, -10);

		// 	glTexCoord2f(1, 0);
		// 	glNormal3f(0, 1, 0);
		// 	glVertex3f(10, -2, 10);

		// 	glTexCoord2f(0, 0);
		// 	glNormal3f(0, 1, 0);
		// 	glVertex3f(-10, -2, 10);
		// 	glEnd();
		// }
	}


	/** Draw the scene. */
	void draw() {
		// The lights must be drawn FIRST, so the other scene elements
		// can get lit!
		pointLight.draw();
		directionalLight.draw();
		spotLight.draw();
		//reset colors
		setDiffuseColor(
			diffuse.getRed(),
			diffuse.getGreen(),
			diffuse.getBlue());

		setAmbientColor(0.2, 0.2, 0.2);

		setSpecularColor(1,1,1);
		setShininess(pow(2.71828183f, 3));

		// Use the texture if desired.
		if (useTexture.getValue()) {

			texture.use();
		} else {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// Use the shader if desired.

		if (useShader.getValue()) {
			// glUseProgram(0);

		GLint brightnessVariableLocation;
		GLint N1VariableLocation;
		GLint N2VariableLocation;
		switch (shaderChoice.getValue()) {
			case 0: // None
				shader.use();
				brightnessVariableLocation = glGetUniformLocation( shader.getID(), "brightness" );
				glUniform1f(brightnessVariableLocation, brightness.getValue());
				break;

			case 1: // Cartoon Shader
				// useShader.setValue(false);
				toonShader.use();
				brightnessVariableLocation = glGetUniformLocation( toonShader.getID(), "brightness" );
				glUniform1f(brightnessVariableLocation, brightness.getValue());
				break;

			case 2: // Schlick Shader
				// shader.use();
				schlickShader.use();

				brightnessVariableLocation = glGetUniformLocation( schlickShader.getID(), "brightness");
				N1VariableLocation = glGetUniformLocation( schlickShader.getID(), "N1");
				N2VariableLocation = glGetUniformLocation( schlickShader.getID(), "N2");
				glUniform1f(brightnessVariableLocation, brightness.getValue());
				glUniform1f(N1VariableLocation, N1.getValue());
				glUniform1f(N2VariableLocation, N2.getValue());
				break;

			case 3: // Texture Blinn Phong
				glUseProgram(0);
				break;

			case 4:
				spotLightShader.use();
				brightnessVariableLocation = glGetUniformLocation(spotLightShader.getID(), "brightness");
				glUniform1f(brightnessVariableLocation, brightness.getValue());
				break;

			}


		// Call a class  that draws our model.
		drawModel();
		// Stop applying shaders to objects.
		// This if-statement makes sure that glUseProgram is not a null
		// function pointer (which it will be if GLEW couldn't initialize).
		if (glUseProgram) {
			glUseProgram(0);
		}

		// Stop applying textures to objects
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
};

/**
 * The program starts here.
 */

int main()
{
	// Tell the FLTK library to let us use OpenGL
	Fl::gl_visual(FL_RGB);

	init_load_curve_file();

	// Instantiate Modeler
	ModelerUserInterface ui;

	// Give Modeler your scene.
	// Modeler will free this pointer when it exits.
	ui.setModel(new Scene());


	// Run the application
    return ui.run();
}
