#include "cloth.h"
#include <FL/gl.h>
#include <math.h>
#include "constraint.h"
#include <iostream>
#include "modelerdraw.h"
#include "FL/glut.H"



Cloth::Cloth(Vec3f origin, float width, float height, int x, int y) {

	// Create grid: origin->(width*x_num, -height*y_num, 0)
	x_num = x;
	y_num = y;

	for (int i = 0; i < x_num; i++) {

		for (int j = 0; j < y_num; j++) {

			Vec3f pos = Vec3f(width * i, // + origin[0],
						-height * j, // + origin[1],
						0.0); // reset z

			 if ((i == 0 && j == 0) || (j == 0 && i == x_num - 1) || (i == 0 && j == y_num - 1) || (i ==x_num - 1 && j == y_num - 1)) {
			 	cpList.push_back(ClothParticle(pos, true));
			} else {
				cpList.push_back(ClothParticle(pos, false));
			}

			//cout << pos << endl;

		}
	}

	// Connect particles: structural constraint & shear constraint 
	// for (int i = 0; i < x_num; ++i)
	// {
	// 	for (int j = 0; j < y_num; ++j)
	// 	{
	// 		int curIndex = j*x_num+i;
	// 		int index; // particle need to connect to the current particle 
	// 		if (i < x_num - 1)
	// 		{
	// 			index = j*x_num+i+1;
	// 			cList.push_back(Constraint(cpList[curIndex], cpList[index]));
	// 		}
	// 		if (j < y_num - 1)
	// 		{
	// 			index = (j+1)*x_num+i;
	// 			cList.push_back(Constraint(cpList[curIndex], cpList[index]));
	// 		}
	// 		// need to do sheer constraint
	// 	}
	// }


}

Cloth::~Cloth() {

	//delete [] cpList;
}

void Cloth::drawCloth() {		
	
	for (int i = 0; i < x_num - 1; ++i) {
		for (int j = 0; j < y_num - 1; ++j) {
			int index = j*x_num+i;
   
   
			Vec3f p1 = cpList[index].getPosition();

			index = (j+1)*x_num+i;

			Vec3f p2 = cpList[index].getPosition();

			index = (j+1)*x_num+i+1;

			Vec3f p3 = cpList[index].getPosition();

			index = j*x_num+i+1;
			
			Vec3f p4 = cpList[index].getPosition();

			Vec3f n1 = Vec3f(0.0);
			Vec3f n2 = Vec3f(0.0);
			Vec3f n3 = Vec3f(0.0);
			Vec3f n4 = Vec3f(0.0);

			Vec3f normal = findNormal(p3, p1, p2);

			n3 += normal;
			n1 += normal;
			n2 += normal;

			normal = findNormal(p4, p3, p2);

			n4 += normal;
			n3 += normal;
			n2 += normal;

			const GLfloat vertices[] = {
			 	p1[0], p1[1], p1[2],    
			 	p2[0], p2[1], p2[2],
				p3[0], p3[1], p3[2],
				p4[0], p4[1], p4[2]
			};

			const GLfloat normals[] = {
				n1[0], n1[1], n1[2],
				n2[0], n2[1], n2[2],
				n3[0], n3[1], n3[2],
				n4[0], n4[1], n4[2],
			};

			const GLubyte indices[] = {0, 1, 2, 0, 2, 3};
			const GLfloat colors[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0 };

			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			
			glNormalPointer( GL_FLOAT, 0, normals);
			glColorPointer(3, GL_FLOAT, 0, colors);
			glVertexPointer(3, GL_FLOAT, 0, vertices);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

	 	}
	 }


 //    glPointSize(6.0f); // when something get checked, enable it, e

    
 //    glBegin(GL_POINTS);
 //    glColor3f(0.0f, 0.0f, 1.0f);
	//  for(int i = 0; i < cpList.size(); i++) {
	//  	glColor3f(0.0f, 0.0f, 1.0f);
		
	// 	glVertex3f(cpList[i].getPosition()[0],cpList[i].getPosition()[1],cpList[i].getPosition()[2] );
	// 	}
	// glEnd();
	
}

Vec3f Cloth::findNormal(Vec3f p1, Vec3f p2, Vec3f p3){

		Vec3f v1 = p2-p1;
		Vec3f v2 = p3-p1;

		return v1 ^ v2;

}

void Cloth::updateForcesAndCollision(float t, Vec3f sphereLoc) {

	for (int i = 0; i < x_num - 1; ++i) {
		for (int j = 0; j < y_num - 1; ++j) {
			int index = j*x_num+i;
			Vec3f v = cpList[index].getPosition()-sphereLoc;
			float length = v.length();

			if (length < .5 + .005) // radius for sphere is fixed & threshold
			{
				// cpList[index].setPosition(cpList[index].getPosition() + v.normalize()*(.5-length));
			}


		}
	}

}

void Cloth::bakeParticles(float t) {

}

/** Start the simulation */
void Cloth::startSimulation(float t)
{

	//prevT = t;
	bake_start_time = 0;

	// These values are used by the UI ...
	// negative bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void Cloth::stopSimulation(float t)
{
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void Cloth::resetSimulation(float t)
{
	bake_start_time = 0;

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Clears out your data structure of baked particles */
void Cloth::clearBaked()
{
	// TODO (baking is extra credit)

}

