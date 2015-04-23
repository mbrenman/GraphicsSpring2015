/*  =================== File Information =================
	File Name: main.cpp
	Description:
	Author: Michael Shah
	Last Modified: 4/24/14

	Purpose: 
	Usage:	

	Further Reading resources: 
	===================================================== */

#include <string.h>
#include <iostream>
#include <fstream>
// You may need to include glee.h here if you are having problems
#include <GL/glew.h>
#include <GL/glui.h>
#include <math.h>
#include "shaderManager.h"
#include "ply.h"


#define PI 3.14

/** These are the live variables passed into GLUI ***/
int main_window;
int wireframe = 0;
int filled = 1;
int interactiveTexture = 0;

float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float eye_pos[] = { 0.0, 0.0, -1.0 };

float xLightPos = 0.0;
float yLightPos = 0.0;
float zLightPos = 0.0;

int mouseX, mouseY, screenWidth, screenHeight;

static float scale = 1;


// Pointer to widget
GLUI *glui2;


/* 	======== Shader Manager ========
	Setup a new shader manager
	============================== */
shaderManager* myShaderManager;

/* 	======== PLY File ========
	Load PLY files
	========================== */
ply* dragonPLY;

/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	glutPostRedisplay();
}


/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
	float xy_aspect;

	screenWidth = x;
	screenHeight = y;

	xy_aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 5.0);

	float projection[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	glUniformMatrix4fv(glGetUniformLocation(myShaderManager->program, "projectionMatrix"), 1, false, projection);


	glutPostRedisplay();
}

void updateMouse(int x, int y){
	//cout << "mouse update: " << x << ", " << y << endl;
	mouseX = x; mouseY = y;
}

void myGlutMotion(int x, int y)
{
	// Get the x and y coordinates
	updateMouse(x, y);

	glUniform4i(glGetUniformLocation(myShaderManager->program, "mouseScreenInfo"), x, y, screenWidth, screenHeight);
}


void myGlutMouse(int button, int button_state, int x, int y)
{
	// Get the x and y coordinates
	updateMouse(x, y);
}

/***************************************** myGlutDisplay() *****************/

// Draw a triangle that represents the light source
void drawLightSource(){
	xLightPos += 0.01f;
	yLightPos = 0.5;
	zLightPos += 0.01f;

	// Actual values that we calculate and send to the shader
	float xLightValue = sin(xLightPos);
	float yLightValue = yLightPos;
	float zLighValue = cos(zLightPos) + 1;

	// Associate our uniform value 'light Vector' in our shader with these 3 values
	// the '3f' at the end means 'lightVector' is a vector of floats
	glUniform3f(glGetUniformLocation(myShaderManager->program, "lightVector"), xLightValue, yLightValue, zLighValue);
}


void myGlutDisplay(void)
{

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glScalef(scale,scale,scale);
	glTranslatef( eye_pos[0], eye_pos[1], eye_pos[2] );
	glMultMatrixf(view_rotate);

	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glUniformMatrix4fv(glGetUniformLocation(myShaderManager->program, "modelviewMatrix"), 1, false, modelview);

	drawLightSource();

		if (filled) {
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glUniform1i(glGetUniformLocation(myShaderManager->program, "usingTexture"), interactiveTexture);  //let the shader know if we're using textures

			if (interactiveTexture == 1) {
				glActiveTexture(GL_TEXTURE0);
				glEnable(GL_TEXTURE_2D);
				//Pass information to our shader to our shader
				glBindTexture(GL_TEXTURE_2D, myShaderManager->layer1ID);
				glUniform1i(glGetUniformLocation(myShaderManager->program, "brick_image"), 0);
			}
			dragonPLY->renderVBO();
			glBindTexture(GL_TEXTURE_2D, 0);  //disable the texture
		}
		
		if (wireframe) {
			glDisable(GL_POLYGON_OFFSET_FILL);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			dragonPLY->renderVBO();
		}
	glutSwapBuffers();
}

/*
	Reclaim memory we've allocated
*/
void onExit()
{
	delete myShaderManager;
	delete dragonPLY;
}



/**************************************** main() ********************/

int main(int argc, char* argv[])
{
	atexit(onExit);

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	main_window = glutCreateWindow("COMP 175 In Class Assignment 10");

	/****************************************/
	/*       Set up OpenGL lighting         */
	/****************************************/

	//No need to do this any more now that we're using shaders... 

	/****************************************/
	/*       Set up OpenGL Shaders          */
	/****************************************/

	//initialize Glew
	GLenum err = glewInit();

	dragonPLY = new ply("./data/bunny.ply");

	// Initialize our shader using our special shader manager class.
	myShaderManager = new shaderManager();

	myShaderManager->initShader("shaders/Bump/bump.vert","shaders/Bump/bump.frag");

	myShaderManager->loadImage("brick.ppm", 0);

	dragonPLY->buildArrays(myShaderManager->program);


	/****************************************/
	/*          Enable z-buferring          */
	/****************************************/

	glEnable(GL_DEPTH_TEST);
	glPolygonOffset(1, 1);

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	//GLUI *glui = GLUI_Master.create_glui("GLUI");

	/*** Create the bottom subwindow ***/
    glui2 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );
    // Create a rotation widget
    GLUI_Rotation *view_rot = new GLUI_Rotation(glui2, "Objects", view_rotate );
    view_rot->set_spin( 1.0 );

    // Navigate our world
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui2, "Objects X", GLUI_TRANSLATION_X, eye_pos );
    trans_x->set_speed( .1 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_y =  new GLUI_Translation( glui2, "Objects Y", GLUI_TRANSLATION_Y, &eye_pos[1] );
    trans_y->set_speed( .1 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z =  new GLUI_Translation( glui2, "Objects Z", GLUI_TRANSLATION_Z, &eye_pos[2] );
    trans_z->set_speed( .1 );

	glui2->add_column(true);

	GLUI_Panel *render_panel = glui2->add_panel("Render");
	new GLUI_Checkbox(render_panel, "Wireframe", &wireframe);
	new GLUI_Checkbox(render_panel, "Filled", &filled);
	new GLUI_Checkbox(render_panel, "Interactive Texture", &interactiveTexture);


	glui2->add_button("Quit", 0, (GLUI_Update_CB)exit);



	glui2->set_main_gfx_window(main_window);
	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);
	glutDisplayFunc(myGlutDisplay);
	glutPassiveMotionFunc(myGlutMotion);
	GLUI_Master.set_glutReshapeFunc(myGlutReshape);
	GLUI_Master.set_glutMouseFunc(myGlutMouse);
	GLUI_Master.set_glutSpecialFunc(NULL);



	glutMainLoop();

	return EXIT_SUCCESS;
}



