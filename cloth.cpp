#include "cloth.h"
#include <FL/gl.h>
#include <math.h>
#include "constraint.h"
#include <iostream>
#include "modelerdraw.h"
#include "FL/glut.H"
#include "model.h"



Cloth::Cloth(Vec3f origin, float width, float height, int x, int y) {

	// Create grid: origin->(width*x_num, -height*y_num, 0)
	x_num = x;
	y_num = y;

	float sectionWidth = width / (float) x;
	float sectionHeight = height / (float) y;

	for (int i = 0; i < x_num; i++) {

		for (int j = 0; j < y_num; j++) {

			Vec3f pos = Vec3f(sectionWidth * i + origin[0], // + origin[0],
						-sectionHeight * j + origin[1], // + origin[1],
						origin[2]; // reset z

			 if ((i == 0 && j == 0) || (j == 0 && i == x_num - 1) || (i == 0 && j == y_num - 1) || (i ==x_num - 1 && j == y_num - 1)) {
			 	cpList.push_back(ClothParticle(pos, true));
			} else {
				cpList.push_back(ClothParticle(pos, false));
			}

			//cout << pos << endl;

		}
	}

// Connect particles: structural constraint & shear constraint 
	for (int i = 0; i < x_num; ++i)
	{
		for (int j = 0; j < y_num; ++j)
		{
			int curIndex = j*x_num+i;
			int index; // particle need to connect to the current particle 
			int index2;
			if (i < x_num - 1) {
				index = j*x_num+i+1;
				cList.push_back(Constraint(cpList[curIndex], cpList[index]));
			}
			if (j < y_num - 1) {
				index = (j+1)*x_num+i;
				cList.push_back(Constraint(cpList[curIndex], cpList[index]));
			}
			if (i < x_num - 1 && j < y_num - 1) {
				index = (j+1)*x_num+i+1;
				cList.push_back(Constraint(cpList[curIndex], cpList[index]));
				index = (j)*x_num+i+1;
				index2 = (j+1)*x_num+i;
				cList.push_back(Constraint(cpList[index], cpList[index2]));
			}
		}
	}

	// Connect particles: bending constraint
	for (int i = 0; i < x_num; ++i)
	{
		for (int j = 0; j < y_num; ++j)
		{
			int curIndex = j*x_num+i;
			int index; // particle need to connect to the current particle 
			int index2;
			if (i < x_num - 2) {
				index = j*x_num+i+2;
				cList.push_back(Constraint(cpList[curIndex], cpList[index]));
			}
			if (j < y_num - 2) {
				index = (j+2)*x_num+i;
				cList.push_back(Constraint(cpList[curIndex], cpList[index]));
			}
			if (i < x_num - 2 && j < y_num - 2) {
				index = (j+2)*x_num+i+2;
				cList.push_back(Constraint(cpList[curIndex], cpList[index]));
				index = (j)*x_num+i+2;
				index2 = (j+2)*x_num+i;
				cList.push_back(Constraint(cpList[index], cpList[index2]));
			}

		}
	}

	//cout << "POINT: " << cpList.size() << endl;
	//cout << "LINES: " << cList.size() << endl;
	


}

Cloth::~Cloth() {

	//delete [] cpList;
	cout << "it's cleaning up the list" << endl;
	cpList.clear();
	cList.clear();
}

void Cloth::drawCloth(RangeProperty sphereCenterX, RangeProperty sphereCenterY, RangeProperty sphereCenterZ) {		
	
	for (int i = 0; i < x_num - 1; ++i) {
		for (int j = 0; j < y_num - 1; ++j) {
			// int index = j*x_num+i;

			ClothParticle* cp1 = getClothParticle( i + 1, j);
			ClothParticle* cp2 = getClothParticle( i, j);
			ClothParticle* cp3 = getClothParticle( i, j + 1);
			ClothParticle* cp4 = getClothParticle( i + 1, j + 1);

			Vec3f p1 = cp1->getPosition();
			Vec3f p2 = cp2->getPosition();
			Vec3f p3 = cp3->getPosition();
			Vec3f p4 = cp4->getPosition();
   			
   			Vec3f n = calculateNormal( cp1->getPosition(), cp2->getPosition(), cp3->getPosition());
   			cp1->addNormal(n);
   			cp2->addNormal(n);
   			cp3->addNormal(n);

   			n = calculateNormal(cp4->getPosition(), cp1->getPosition(), cp3->getPosition());

   			cp4->addNormal(n);
   			cp1->addNormal(n);
   			cp3->addNormal(n);

   			Vec3f n1 = cp1->getNormal();
   			Vec3f n2 = cp2->getNormal();
   			Vec3f n3 = cp3->getNormal();
   			Vec3f n4 = cp4->getNormal();


			// Vec3f p1 = cpList[index].getPosition();

			// index = (j+1)*x_num+i;

			// Vec3f p2 = cpList[index].getPosition();

			// index = (j+1)*x_num+i+1;

			// Vec3f p3 = cpList[index].getPosition();

			// index = j*x_num+i+1;
			
			// Vec3f p4 = cpList[index].getPosition();

			// Vec3f n1 = Vec3f(0.0);
			// Vec3f n2 = Vec3f(0.0);
			// Vec3f n3 = Vec3f(0.0);
			// Vec3f n4 = Vec3f(0.0);

			// Vec3f normal = findNormal(p3, p1, p2);

			// n3 += normal;
			// n1 += normal;
			// n2 += normal;

			// normal = findNormal(p4, p3, p2);

			// n4 += normal;
			// n3 += normal;
			// n2 += normal;

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

			const GLubyte indices[] = {0, 1, 2, 3, 0, 2};
			const GLfloat colors[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0 };

			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			
			glNormalPointer( GL_FLOAT, 0, normals);
			glColorPointer(3, GL_FLOAT, 0, colors);
			glVertexPointer(3, GL_FLOAT, 0, vertices);

			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

	 	}
	 }

	// Draw the default sphere for collision
	glPushMatrix();
		glTranslatef(sphereCenterX.getValue(), sphereCenterY.getValue(), sphereCenterZ.getValue());
		glutSolidSphere(.5, 20, 20); // the 20's are arbitary
		ballLoc = Vec3f(sphereCenterX.getValue(), sphereCenterY.getValue(), sphereCenterZ.getValue());
	glPopMatrix();

	// Draw the points;
	 for(int i = 0; i < cpList.size(); i++) {
	 	cpList[i].drawClothParticle();
	 }

	// Draw the constraints
	for (int i = 0; i < cList.size(); i++) {
		cList[i].drawConstraint();
	}

	// Draw the fan
	drawFan();
	

	
}

void Cloth::drawFan() {
    
    glBegin(GL_TRIANGLES);//start drawing triangles
      glVertex3f(-.5f,-.866f,-1.5f);//triangle one first vertex
      glVertex3f(.5f,-.866f,-1.5f);//triangle one second vertex
      glVertex3f(0.0f,0.0f,-1.5f);//triangle one third vertex
      //drawing a 2nd triangle
      glVertex3f(0.0f,0.0f,-1.5f);//triangle two first vertex
      glVertex3f(-1.0f,0.0f,-1.5f);//triangle two second vertex
      glVertex3f(-0.5f,0.866f,-1.5f);//triangle two third vertex
      //drawing a 3nd triangle
      glVertex3f(0.0f,0.0f,-1.5f);//triangle two first vertex
      glVertex3f(1.0f,0.0f,-1.5f);//triangle two second vertex
      glVertex3f(0.5f,0.866f,-1.5f);//triangle two third vertex
    glEnd();//end drawing of triangles

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
				cpList[index].setPosition(cpList[index].getPosition() + v*(.5-length)); // may have to normalize
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

Vec3f Cloth::calculateNormal(Vec3f p1, Vec3f p2, Vec3f p3) {

	Vec3f v1 = p2 - p1;
	Vec3f v2 = p3 - p1;

	return v1 ^ v2;
}


