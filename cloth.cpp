#include "cloth.h"
#include <FL/gl.h>
#include <math.h>
#include "constraint.h"
#include <iostream>
#include "modelerdraw.h"
#include "FL/glut.H"
#include "model.h"

using namespace std;


static float prevT;

Cloth::Cloth() {
	simulate = false;
	init(Vec3f(0, 0, 0), 7.0, 7.0, 50, 50);
	// init(Vec3f(-2.0, 3.5, 2.0), 15.0, 15.0, 15, 15);
}
Cloth::Cloth(Vec3f origin, float width, float height, int x, int y) {

	// Create grid: origin->(width*x_num, -height*y_num, 0)
	init(origin, width, height, x, y);


}

void Cloth::init(Vec3f origin, float width, float height, int x, int y) {
	x_num = x;
	y_num = y;

	float sectionWidth = width / (float) x_num;
	float sectionHeight = height / (float) y_num;

	// cout << sectionWidth << endl;
	// cpList.clear();
	// cList.clear();

	// cout << y_num << endl;
	for (int i = 0; i < x_num; i++) {

		for (int j = 0; j < y_num; j++) {

			Vec3f pos = Vec3f(sectionWidth * i + origin[0], // + origin[0],
						-sectionHeight * j + origin[1], // + origin[1],
						origin[2]); // reset z

			//pin three top left points and three top right points
			 if (j == 0 && (i < 4 || i > y_num - 4) ){
			 	// || (i == 0 && j == y_num - 1) || (i ==x_num - 1 && j == y_num - 1)) {
			 	cpList.push_back(new ClothParticle(pos, true));
			} else {
				cpList.push_back(new ClothParticle(pos, false));
			}

			//cout << pos << endl;


		}
	}

// Connect particles: structural constraint & shear constraint 
	// for (int i = 0; i < x_num; i++)
	// {
	// 	for (int j = 0; j < y_num; j++)
	// 	{
	// 		// int curIndex = j*x_num+i;
	// 		// int index; // particle need to connect to the current particle 
	// 		// int index2;
	// 		if (i < x_num - 1) {
	// 			// index = j*x_num+i+1;
	// 			// cList.push_back(Constraint(cpList[curIndex], cpList[index]));
	// 			cList.push_back(Constraint( getClothParticle(i, j), getClothParticle(i + 1, j)));
	// 		}
	// 		if (j < y_num - 1) {
	// 			// index = (j+1)*x_num+i;
	// 			// cList.push_back(Constraint(cpList[curIndex], cpList[index]));
	// 			cList.push_back(Constraint(getClothParticle(i, j), getClothParticle(i, j + 1)));
	// 		}
	// 		if (i < x_num - 1 && j < y_num - 1) {
	// 			// index = (j+1)*x_num+i+1;
	// 			// cList.push_back(Constraint(cpList[curIndex], cpList[index]));
	// 			cList.push_back(Constraint(getClothParticle(i, j), getClothParticle(i + 1, j + 1)));
	// 			// index = (j)*x_num+i+1;
	// 			// index2 = (j+1)*x_num+i;
	// 			// cList.push_back(Constraint(cpList[index], cpList[index2]));
	// 			cList.push_back(Constraint(getClothParticle(i + 1, j), getClothParticle(i, j + 1)));
	// 		}

			
	// 	}
	// }

	for(int i = 0; i < x_num; i++) {
		for(int j = 0; j < y_num; j++) {
			if (i < x_num - 1) {
				// index = j*x_num+i+1;
				// cList.push_back(Constraint(cpList[curIndex], cpList[index]));
				cList.push_back(new Constraint( getClothParticle(i, j), getClothParticle(i + 1, j)));
			}
			if (j < y_num - 1) {
				// index = (j+1)*x_num+i;
				// cList.push_back(Constraint(cpList[curIndex], cpList[index]));
				cList.push_back(new Constraint(getClothParticle(i, j), getClothParticle(i, j + 1)));
			}
			if (i < x_num - 1 && j < y_num - 1) {
				// index = (j+1)*x_num+i+1;
				// cList.push_back(Constraint(cpList[curIndex], cpList[index]));
				cList.push_back(new Constraint(getClothParticle(i, j), getClothParticle(i + 1, j + 1)));
				// index = (j)*x_num+i+1;
				// index2 = (j+1)*x_num+i;
				// cList.push_back(Constraint(cpList[index], cpList[index2]));
				cList.push_back( new Constraint(getClothParticle(i + 1, j), getClothParticle(i, j + 1)));
			}
		}
	}

	for(int i = 0; i < x_num; i++) {
		for(int j = 0; j < y_num; j++) {
			if (i < x_num - 2) {
				// index = j*x_num+i+2;
				cList.push_back(new Constraint(getClothParticle(i, j), getClothParticle(i + 2, j)));
			}
			if (j < y_num - 2) {
				// index = (j+2)*x_num+i;
				cList.push_back(new Constraint(getClothParticle(i, j), getClothParticle(i, j + 2)));
			}
			if (i < x_num - 2 && j < y_num - 2) {
				// index = (j+2)*x_num+i+2;
				cList.push_back(new Constraint(getClothParticle(i, j), getClothParticle(i + 2, j + 2)));
				// index = (j)*x_num+i+2;
				// index2 = (j+2)*x_num+i;
				cList.push_back(new Constraint(getClothParticle(i + 2, j), getClothParticle(i, j + 2)));
			}
		}
	}

	//drawCloth(ballLoc[0], ballLoc[1], ballLoc[2]);
	// // Connect particles: bending constraint
	// for (int i = 0; i < x_num; ++i)
	// {
	// 	for (int j = 0; j < y_num; ++j)
	// 	{
	// 		int curIndex = j*x_num+i;
	// 		int index; // particle need to connect to the current particle 
	// 		int index2;
	// 		if (i < x_num - 2) {
	// 			index = j*x_num+i+2;
	// 			cList.push_back(Constraint(cpList[curIndex], cpList[index]));
	// 		}
	// 		if (j < y_num - 2) {
	// 			index = (j+2)*x_num+i;
	// 			cList.push_back(Constraint(cpList[curIndex], cpList[index]));
	// 		}
	// 		if (i < x_num - 2 && j < y_num - 2) {
	// 			index = (j+2)*x_num+i+2;
	// 			cList.push_back(Constraint(cpList[curIndex], cpList[index]));
	// 			index = (j)*x_num+i+2;
	// 			index2 = (j+2)*x_num+i;
	// 			cList.push_back(Constraint(cpList[index], cpList[index2]));
	// 		}

	// 	}
	// }

	//cout << "POINT: " << cpList.size() << endl;
	//cout << "LINES: " << cList.size() << endl;
}

Cloth::~Cloth() {

	//delete [] cpList;
	// cout << "it's cleaning up the list" << endl;
	// cpList.clear();
	// cList.clear();
}

void Cloth::drawCloth(float xx, float yy, float zz) {		
	
	// cout << xx << endl;
	// cout << cpList.size() << endl;
	// cout << y_num << endl;
	for (int i = 0; i < x_num - 1; ++i) {
		for (int j = 0; j < y_num - 1; ++j) {
			// int index = j*x_num+i;
			// cout << "drawing" << endl;
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
			const GLfloat colors[] = { 10, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0 };

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


	// Draw the default sphere for collision
	glPushMatrix();
		glTranslatef(xx, yy, zz);
		glutSolidSphere(.5, 20, 20); // the 20's are arbitary
		ballLoc = Vec3f(xx, yy, zz);
	glPopMatrix();

	// // Draw the points;
	//  for(int i = 0; i < cpList.size(); i++) {
	//  	cpList[i]->drawClothParticle();
	//  }

	// // Draw the constraints
	// for (int i = 0; i < cList.size(); i++) {
	// 	cList[i]->drawConstraint();
	// }

	// Draw the fan
	//drawFan();

	

	
}

// void Cloth::drawCloth() {
// 	drawCloth(x, y, z);
// }

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

void Cloth::updateForcesAndCollision(float t, float x, float y, float z, float v, int num) {

	float deltaT = t - prevT;
	prevT = t;
	ballLoc = Vec3f(x, y, z);
	windV = v;
	windDir = num; // case number: 1 2 3 4 represent direction cases: in, out, left, right
	Vec3f dir;
	if(simulate) {
		
		// add gravity to all particles
		for(int i = 0; i < cpList.size(); i++) {
			cpList[i]->addAcceleration(Vec3f( 0.0, -9.81, 0.0) * deltaT);

		}

		// //add wind force to all particles
		//cout << "WIND SPEED IS: " << windV << endl;
		for(int i = 0; i < x_num - 1; i++) {
			for(int j = 0; j < y_num - 1; j++) {
				switch (windDir) {
					case 1: // direction is in
						dir = Vec3f(0.0, 0.0, -1.0);
						break;
					case 2: // direction is out
						dir = Vec3f(0.0, 0.0, 1.0);
						break;
					case 3: // direction is left
						dir = Vec3f(1.0, 0.0, 0.0);
						break;
					case 4: // right
						dir = Vec3f(-1.0, 0.0, 0.0);
						break;
				}
				addWind(getClothParticle(i + 1, j), getClothParticle(i, j), getClothParticle(i, j + 1),
				dir * deltaT * windV);
				addWind(getClothParticle(i + 1, j + 1), getClothParticle(i + 1, j), getClothParticle(i, j + 1),
				dir * deltaT * windV);
			}
		}

		// check that they satisfy their constraints (check 16 times)
		for(int i = 0; i < 16; i++) {
			for(int j = 0; j < cList.size(); j++) {
				cList[j]->satisfyConstraints();
			}
		}

		//update positions of cloth particles
		for(int i = 0; i < cpList.size(); i++) {
			cpList[i]->update(deltaT);
		}

		checkForCollision();

		drawCloth(x, y, z);

	}

	// cout << cpList[5]->getPosition()[2] << endl;



}

void Cloth::addWind(ClothParticle* p1, ClothParticle* p2, ClothParticle* p3, Vec3f dir){

	Vec3f n = calculateNormal(p1->getPosition(), p2->getPosition(), p3->getPosition());
	Vec3f direction = n;
	direction.normalize();
	Vec3f acc = n * (direction * dir) * 50;
	p1->addAcceleration(acc);
	p2->addAcceleration(acc);
	p3->addAcceleration(acc);
}

void Cloth::checkForCollision() {
	for (int i = 0; i < x_num ; ++i) {
			for (int j = 0; j < y_num ; ++j) {
				// int index = j*x_num+i;
				Vec3f v = getClothParticle(i, j)->getPosition() - ballLoc;
				float length = v.length();

				v.normalize();

				if (length < .5 + .04) // radius for sphere is fixed & threshold
				{
					getClothParticle(i, j)->setPosition(getClothParticle(i, j)->getPosition() + v *((.5 + .04)-length)); // may have to normalize
					//getClothParticle(i, j)->updatePosition(v*(.5-length));
				}



			}
		}
		

}

void Cloth::bakeParticles(float t) {

}

/** Start the simulation */
void Cloth::startSimulation(float t)
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// glLoadIdentity();	
	prevT = t;
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


