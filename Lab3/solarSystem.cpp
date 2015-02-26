#include <GL/glui.h>
#include "solarSystem.h"

solarSystem::solarSystem(){

}

solarSystem::~solarSystem(){

}

// Render this with push and pop operations
// or alternatively implement helper functions
// for each of the planets with parameters.
void solarSystem::render(){
	
	// Some ideas for constants that can be used for
	// rotating the planets.
	static float rotation = 0.1;
	static float orbitX = 0.15;
	static float orbitSpeed = 0.2;
	static float moonOrbitX = 0.1;
	static float moonOrbitY = 0.1;
	static float moonOrbitSpeed = 4;

	// The Sun
		glPushMatrix();
			glRotatef(rotation,0,1,0);
			glColor3f(0.96f,0.85f,0.26f);
			glutSolidSphere(1.2,10,10);
		glPopMatrix();
	// Add more planets, moons, and rings here!	

	//Planet 1
		glPushMatrix();
			glRotatef(rotation,0,1,0);
			glTranslatef(5, 0, 0);
			glColor3f(0.26f,0.89f,0.76f);
			glutSolidSphere(.25,15,15);

			//Planet 1 Moon
			glPushMatrix();
				glRotatef(-rotation,0,1,0);
				glRotatef(rotation * moonOrbitSpeed,1,0,0);
				glTranslatef(0, 1, 0);
				glColor3f(0.90f,0.19f,0.76f);
				glutSolidSphere(.15,15,15);
			glPopMatrix();

		glPopMatrix();
	
	//Planet 2
		glPushMatrix();
			glRotatef(rotation * 0.1,0,1,0);
			glTranslatef(7, 0, 7);
			glColor3f(0.96f,0.12f,0.16f);
			glutSolidSphere(3,15,15);
		glPopMatrix();

	//Planet 3
		glPushMatrix();
			glRotatef(rotation,0,1,0);
			glTranslatef(-3, 0, 0);
			glColor3f(0.86f,0.89f,0.76f);
			glutSolidSphere(.55,15,15);

			//Planet 3 Moon
			glPushMatrix();
				glRotatef(rotation * moonOrbitSpeed * 0.5,1,1,1);
				glTranslatef(1, 1, 0);
				glColor3f(0.05f,0.99f,0.76f);
				glutSolidSphere(.25,15,15);


				//Planet 3 Moon Moon
				glPushMatrix();
					glRotatef(rotation * moonOrbitSpeed * 1.4,1,0,0);
					glTranslatef(0, 0, 1);
					glColor3f(0.30f,0.29f,0.76f);
					glutSolidSphere(.1,15,15);
				glPopMatrix();

			glPopMatrix();

			//Planet 3 Moon 2
			glPushMatrix();
				glRotatef(-rotation,0,1,0);
				glRotatef(rotation * moonOrbitSpeed,1,0,0);
				glTranslatef(0, 1, 0);
				glColor3f(0.90f,0.19f,0.16f);
				glutSolidSphere(.05,15,15);
			glPopMatrix();

		glPopMatrix();

	rotation+= 0.3;
	orbitX+=orbitSpeed;
	moonOrbitX+=moonOrbitSpeed;
	moonOrbitY+=moonOrbitSpeed;

}