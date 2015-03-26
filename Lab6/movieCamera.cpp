/*  =================== File Information =================
  File Name: movieCamera.cpp
  Description:
  Author: 

  Purpose:
  Examples:


Relevant keywords and OpenGL calls to (understand/complete/create cool effects) for this lab:
-glFrustrum
-glDepthRange
-glMultMatrixf
-glTranslate/glRotate/glScale
-glOrtho
-OpenGL field of view


  ===================================================== */
#include <math.h>
#include <iostream>
#include "movieCamera.h"
#include "Algebra.h"

// Static initializers
float movieCamera::rotate_around = 0;


	/*  ===============================================
	      Desc: Default Constructor
	      Precondition: Setup your matrix to the identity
	      Postcondition:
    	=============================================== */ 
movieCamera::movieCamera(){
	}

	/*  ===============================================
	      Desc: Default Destructor
	      Precondition:
	      Postcondition:
    	=============================================== */ 
movieCamera::~movieCamera(){

}

/*
	Implement the clipping plane
*/
/*  ===============================================
      Desc: Zoom in on an object.  Optionally clip out objects that are 'far' away from this object
      Precondition:
      Postcondition:
	=============================================== */ 
void movieCamera::closeUp(float your_x, float your_y, float your_z, float near, float far){

}

/*
*/
void movieCamera::panAround(float focus_x, float focus_y, float focus_z, float t) {
	RotateW(0);
	float radians = PI/2 * t - PI/2;

	std::cout << std::fixed << "t: " << t << std::endl;

	Point eye(
		3.0 * cos(radians) + focus_x,
		focus_y,
		-2.0 * sin(radians) + focus_z
		);
		
	Point focus(focus_x, focus_y, focus_z);
	Vector up(0, 1, 0);
	Orient(eye, focus, up);
}

void movieCamera::frontToDino(float jeep_x, float jeep_y, float jeep_z,
							  float dino_x, float dino_y, float dino_z,
							  float t) {
	std::cout << std::fixed << "t: " << t << std::endl;

	float radians = PI/2 * t - PI/2;

	dino_y += .7; //
	dino_x += 3;

	Point eye(
		((1 - t) * (jeep_x + 3.0f)) + (t * dino_x),
		((1 - t) * (jeep_y)) + (t * dino_y),
		((1 - t) * (jeep_z)) + (t * dino_z)
		);

	Point focus(((1 - t) * jeep_x + t * dino_x),
				((1 - t) * jeep_y + t * dino_y),
				((1 - t) * jeep_z + t * dino_z));
	Vector up(0, 1, 0);
	Orient(eye, focus, up);
}

void movieCamera::dinoShake(float dino_x, float dino_y, float dino_z, float t)
{
	dino_y += .7; //
	dino_x += 3;

	std::cout << std::fixed << "t: " << t << std::endl;

	float radians1 = 12 * PI * t;
	float radians2 = 8 * PI * t;
	float radians3 = 24 * PI * t;

	Point eye(
		dino_x + 0.25f * sin(radians1),
		dino_y + 0.25f * sin(radians2),
		dino_z + 0.25f * sin(radians3)
		);

	dino_y -= .7; //
	dino_x -= 3;

	Point focus(dino_x,
				dino_y,
				dino_z);

	Vector up(0, 1, 0);
	Orient(eye, focus, up);
}

void movieCamera::dinoWalk(float jeep_x, float jeep_y, float jeep_z,
					  float dino_x, float dino_y, float dino_z,
					  float t) {

	float radiansStep = 5 * PI * t;

	std::cout << std::fixed << "t: " << t << std::endl;

	//Look from dino's eye (walking)
	Point eye(
		dino_x + 3,
		dino_y + .7 + 0.5 * fabs(sin(radiansStep)),
		dino_z);

	Point focus(jeep_x,
				jeep_y,
				jeep_z);

	RotateW(15 * cos(radiansStep));

	Vector up(0, 1, 0);
	Orient(eye, focus, up);
}

void movieCamera::charge(float jeep_x, float jeep_y, float jeep_z,
					  float dino_x, float dino_y, float dino_z,
					  float t) {
	float radiansStep = 5 * PI * (t + 1);
	float radiansShake = 12 * PI * (t + 1);

	std::cout << std::fixed << "t: " << t << std::endl;

	//Look from dino's eye (walking)
	Point eye(
		dino_x + 3,
		dino_y + .7 + 0.5 * fabs(sin(radiansStep)),
		dino_z + 0.2 * sin(radiansShake));

	Point focus(jeep_x,
				jeep_y,
				jeep_z);

	RotateW(15 * cos(radiansStep));

	Vector up(0, 1, 0);
	Orient(eye, focus, up);
}
/*
	Change the field of view
*/
/*  ===============================================
      Desc: Change the viewing angle of the camera lense
      Precondition:
      Postcondition:
	=============================================== */ 
void movieCamera::wideAngle(float _fov){

}

/*
	Set the camera to an orthogonal view
*/
/*  ===============================================
  Desc: Remove the depth attribute from the camera.
  Precondition:
  Postcondition:
=============================================== */ 
void movieCamera::orthogonal(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble nearVal, GLdouble farVal){

}

/*  ===============================================
      Desc: Render the scene in a normal perspective field of view.
            The parameters for this function match that of the OpenGL Utility library.
      Precondition:
      Postcondition:
	=============================================== */ 
void movieCamera::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar){

}
/*  ===============================================
      Desc: Set the camera to a 'look at' a target from a specified position and follow it.
            your_x would be your position, and the target could be a moving or non-moving target
      Precondition:
      Postcondition:
	=============================================== */ 
void movieCamera::follow(float your_x, float your_y, float your_z,
						float target_x, float target_y, float target_z,
						float up_x, float up_y, float up_z){
	Point eye(your_x, your_y, your_z);
	Point focus(target_x, target_y, target_z);
	Vector up(up_x, up_y, up_z);

	Orient(eye, focus, up);
	std::cout << "follow started" << std::endl;
}
/*  ===============================================
      Desc: Spin around a point in space at a distance(i.e. "radius")
      Precondition:
      Postcondition:
	=============================================== */ 
void movieCamera::spinAroundPoint(float your_x, float your_y, float your_z, float distance){
	rotate_around += 1;

	/* Your code here */

	if(rotate_around>360){ rotate_around = 0; }
}
