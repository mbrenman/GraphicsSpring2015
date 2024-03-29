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
	
	// Some ideas for variables that can be used for
	// rotating the planets.
	static float rotation = 0.1;
	static float orbitX = 0.15;
	static float orbitSpeed = 0.1;
	static float moonOrbitX = 0.1;
	static float moonOrbitY = 0.1;
	static float moonOrbitSpeed = 0.2;

	// The Sun
		glPushMatrix();
			renderPlanet(3, orbitX, 0.2, rotation * 2);
			renderPlanet(5, orbitX * 0.5, 0.6, rotation * 1.5);
			glRotatef(rotation, 0, 1, 0);
			glColor3f(0.96f,0.85f,0.26f);
			glutSolidSphere(1.2,10,10);
		glPopMatrix();

	// Add more planets, moons, and rings here!	
		

	rotation+= 0.01;
	orbitX+=orbitSpeed;
	moonOrbitX+=moonOrbitSpeed;
	moonOrbitY+=moonOrbitSpeed;

}

void solarSystem::renderPlanet(float orbitrad, float orbitdeg, float size, float rotdeg) {
	glPushMatrix();
	glRotatef(orbitdeg, 0, 1, 0);
	glTranslatef(orbitrad, 0, 0);
	renderMoon(orbitrad*0.3, orbitdeg*0.5, size*0.3, rotdeg);
	glRotatef(rotdeg, 0, 1, 0);
	glColor3f(0.26f, 0.26f, 0.96f);
	glutSolidSphere(size, 10, 10);
	glPopMatrix();
}

void solarSystem::renderMoon(float orbitrad, float orbitdeg, float size, float rotdeg) {
	glPushMatrix();
	glRotatef(orbitdeg, 0, 1, 0);
	glTranslatef(orbitrad, 0, 0);
	glRotatef(rotdeg, 0, 1, 0);
	glColor3f(0.26f, 0.96f, 0.26f);
	glutSolidSphere(size, 10, 10);
	glPopMatrix();
}