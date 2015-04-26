#define NUM_OPENGL_LIGHTS 8

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <GL/glui.h>
#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "SceneParser.h"
#include "Camera.h"

using namespace std;

/** These are the live variables passed into GLUI ***/
int  isectOnly = 1;

int	 camRotU = 0;
int	 camRotV = 0;
int	 camRotW = 0;
int  viewAngle = 45;
float eyeX = 2;
float eyeY = 2;
float eyeZ = 2;
float lookX = -2;
float lookY = -2;
float lookZ = -2;

/** These are GLUI control panel objects ***/
int  main_window;
string filenamePath = "data/tests/work.xml";
// GLUI_EditText* filenameTextField = NULL;
GLubyte* pixels = NULL;
int pixelWidth = 0, pixelHeight = 0;
int screenWidth = 0, screenHeight = 0;

/** these are the global variables used for rendering **/
Cube* cube = new Cube();
Cylinder* cylinder = new Cylinder();
Cone* cone = new Cone();
Sphere* sphere = new Sphere();
SceneParser* parser = NULL;
Camera* camera = new Camera();

void setPixel(GLubyte* buf, int x, int y, int r, int g, int b) {
	buf[(y*pixelWidth + x) * 3 + 0] = (GLubyte)r;
	buf[(y*pixelWidth + x) * 3 + 1] = (GLubyte)g;
	buf[(y*pixelWidth + x) * 3 + 2] = (GLubyte)b;
}

struct shapeData {
	PrimitiveType type;
	SceneMaterial material;
	Matrix composite;
};

typedef std::list<shapeData> list_shapeData;

void setupCamera();
void updateCamera();
Point getEyePoint();
Vector generateRay(int pixelX, int pixelY);
Point getIsectPointWorldCoord(Point eye, Vector ray, double t);
Point getIntensity(Point eyePt, Vector ray, list_shapeData objects, SceneGlobalData globalData, int numRec);

void flattenScene(SceneNode *root, list_shapeData &list, Matrix cmtm) {
	/*
	iterators are initialized outside for loop for gcc compatibility. apparently
	its an issue. Also, I intentionally used the prefix ++ operator, it allegedly
	is more portable. If any of this doesn't work on your computer feel free to
	modify it.
	*/
	std::vector<SceneTransformation*>::iterator it;
	std::vector<ScenePrimitive*>::iterator itp;
	std::vector<SceneNode*>::iterator itc;


	for (it = root->transformations.begin(); it != root->transformations.end(); ++it) {
		SceneTransformation *next = *it;

		switch (next->type){
		case TRANSFORMATION_TRANSLATE:
			cmtm = cmtm * trans_mat(next->translate);
			break;
		case TRANSFORMATION_SCALE:
			cmtm = cmtm * scale_mat(next->scale);
			break;
		case TRANSFORMATION_ROTATE:
			cmtm = cmtm * rot_mat(next->rotate, next->angle);
			break;
		case TRANSFORMATION_MATRIX:
			cmtm = cmtm * next->matrix;
			break;
		}
	}

	for (itp = root->primitives.begin(); itp != root->primitives.end(); ++itp) {
		ScenePrimitive *next = *itp;
		shapeData entry;
		entry.composite = cmtm;
		entry.material = next->material;
		entry.type = next->type;

		list.push_back(entry);
	}

	for (itc = root->children.begin(); itc != root->children.end(); ++itc){
		SceneNode *next = *itc;
		flattenScene(next, list, cmtm);
	}
}

void callback_start(int id) {
	cout << "start button clicked!" << endl;

	if (parser == NULL) {
		cout << "no scene loaded yet" << endl;
		return;
	}

	pixelWidth = screenWidth;
	pixelHeight = screenHeight;

	updateCamera();

	if (pixels != NULL) {
		delete pixels;
	}
	pixels = new GLubyte[pixelWidth  * pixelHeight * 3];
	memset(pixels, 0, pixelWidth  * pixelHeight * 3);

	cout << "(w, h): " << pixelWidth << ", " << pixelHeight << endl;

	SceneNode* root = parser->getRootNode();
	
	SceneGlobalData globalData;
	parser->getGlobalData(globalData);

	//Matrix compositeMatrix;
	list_shapeData objects;
	flattenScene(root, objects, Matrix());

	Point eyePt = getEyePoint();

	for (int i = 0; i < pixelWidth; i++) {
		for (int j = 0; j < pixelHeight; j++) {
			Vector ray = generateRay(i, j);

			Point intensity = getIntensity(eyePt, ray, objects, globalData, 1);
			setPixel(pixels, i, pixelHeight - j - 1, intensity.at(0), intensity.at(1), intensity.at(2));

		}
	}
	glutPostRedisplay();
}

Point getIntensity(Point eyePt, Vector ray, list_shapeData objects, SceneGlobalData globalData, int numRec){
	Shape::intersect_info info;
	double min_t = -1;
	Vector norm = Vector();
	Shape *min_shape = NULL;

	Matrix min_matrix;
	PrimitiveType min_type;
	SceneMaterial min_material;

	std::list<shapeData>::iterator it;
	for (it = objects.begin(); it != objects.end(); ++it){
		shapeData obj = *it;
		Shape* curr_shape;
		switch (obj.type) {
			case SHAPE_SPHERE:
			curr_shape = sphere;
			break;
			case SHAPE_CONE:
			curr_shape = cone;
			break;
			case SHAPE_CUBE:
			curr_shape = cube;
			break;
			case SHAPE_CYLINDER:
			curr_shape = cylinder;
			break;
			default:
			curr_shape = NULL;
			break;
		}
		if (curr_shape != NULL) {
			info = curr_shape->Intersect(eyePt, ray, obj.composite);
			if ((min_t < 0 || info.t < min_t) && (info.t > 0)) {
				min_t = info.t;
				norm = info.normal;
				min_matrix = obj.composite;
				min_type = obj.type;
				min_shape = curr_shape;
				min_material = obj.material;
			}
		}
	}
	if (min_t != -1) {
		if (isectOnly) {
			return Point(255, 255, 255);
		}	
		else {
					//Compute Normal
			norm = transpose(invert(min_matrix)) * norm;
			norm.normalize();

					//Find color
			double r = globalData.ka * (double)min_material.cAmbient.r;
			double g = globalData.ka * (double)min_material.cAmbient.g;
			double b = globalData.ka * (double)min_material.cAmbient.b;

			Point intersectPoint = getIsectPointWorldCoord(eyePt, ray, min_t);

			int numLights = parser->getNumLights();
			for (int m = 0; m < numLights; m++) {
				SceneLightData light;
				parser->getLightData(m, light);

				Vector L = light.pos - intersectPoint;
				L.normalize();

				double normLightDot = dot(norm, L);
				normLightDot = normLightDot > 0 ? normLightDot : 0;

				double dr = (double)globalData.kd * (double)min_material.cDiffuse.r * (double)light.color.r * (double)normLightDot;
				double dg = (double)globalData.kd * (double)min_material.cDiffuse.g * (double)light.color.g * (double)normLightDot;
				double db = (double)globalData.kd * (double)min_material.cDiffuse.b * (double)light.color.b * (double)normLightDot;

				r += (dr > 0) ? dr : -dr;
				g += (dg > 0) ? dg : -dg;
				b += (db > 0) ? db : -db;

			}

			if (numRec > 0) {
				//Reflection
				Vector reflected = ray - 2 * dot(ray, norm) * norm;
				reflected.normalize();

				Point ref_color = getIntensity(intersectPoint, reflected, objects, globalData, numRec - 1);

				double reflected_r = (double)globalData.ks * (double)min_material.cReflective.r * ref_color.at(0);
				double reflected_g = (double)globalData.ks * (double)min_material.cReflective.g * ref_color.at(1);
				double reflected_b = (double)globalData.ks * (double)min_material.cReflective.b * ref_color.at(2);

				r += (reflected_r > 0) ? reflected_r : -reflected_r;
				g += (reflected_g > 0) ? reflected_g : -reflected_g;
				b += (reflected_b > 0) ? reflected_b : -reflected_b;
			}

			//cap at 1
			r = (r > 1) ? 1 : r;
			g = (g > 1) ? 1 : g;
			b = (b > 1) ? 1 : b;

			r = ((double)r * 255.0f);
			g = ((double)g * 255.0f);
			b = ((double)b * 255.0f);

			return Point(r, g, b);
			// setPixel(pixels, i, pixelHeight - j - 1, r, g, b);
		}
	}
	else {
		return Point(0, 0, 0);
		// setPixel(pixels, i, pixelHeight-j-1, 0, 0, 0);
	}
}



Point getEyePoint();

Vector generateRay(int pixelX, int pixelY) {
	double filmPointX = -1 + (2 * (double)pixelX / (double)pixelWidth);
	double filmPointY = -1 * (-1 + (2 * (double)pixelY / (double)pixelHeight));

	Point filmPoint = Point(filmPointX, filmPointY, -1);
	Point eyePoint = Point(0, 0, 0);
	Matrix filmtoworld = invert(camera->GetScaleMatrix() * camera->GetModelViewMatrix());

	filmPoint = filmtoworld * filmPoint;
	eyePoint = filmtoworld * eyePoint;

	Vector ray = filmPoint - eyePoint;

	ray.normalize();
	return ray;
}

Point getEyePoint() {
	return camera->GetEyePoint();
}

Point getIsectPointWorldCoord(Point eye, Vector ray, double t) {
	Point p = eye + t * ray;
	return p;
}


void callback_load(int id) {
	char curDirName [2048];
	 // if (filenameTextField == NULL) {
	 // 	return;
	 // }
	 // printf ("%s\n", filenamePath);

	if (parser != NULL) {
		delete parser;
	}
	parser = new SceneParser (filenamePath);
	cout << "success? " << parser->parse() << endl;

	setupCamera();
}


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

	xy_aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);
	camera->SetScreenSize(x, y);

	screenWidth = x;
	screenHeight = y;

	glutPostRedisplay();
}


/***************************************** setupCamera() *****************/
void setupCamera()
{
	SceneCameraData cameraData;
	parser->getCameraData(cameraData);

	camera->Reset();
	camera->SetViewAngle(cameraData.heightAngle);
	if (cameraData.isDir == true) {
		camera->Orient(cameraData.pos, cameraData.look, cameraData.up);
	}
	else {
		camera->Orient(cameraData.pos, cameraData.lookAt, cameraData.up);
	}

	viewAngle = camera->GetViewAngle();
	Point eyeP = camera->GetEyePoint();
	Vector lookV = camera->GetLookVector();
	eyeX = eyeP[0];
	eyeY = eyeP[1];
	eyeZ = eyeP[2];
	lookX = lookV[0];
	lookY = lookV[1];
	lookZ = lookV[2];
	camRotU = 0;
	camRotV = 0;
	camRotW = 0;
	GLUI_Master.sync_live_all();
}

void updateCamera()
{
	camera->Reset();

	Point guiEye (eyeX, eyeY, eyeZ);
	Point guiLook(lookX, lookY, lookZ);
	camera->SetViewAngle(viewAngle);

	Vector up = camera->GetUpVector();
	camera->Orient(guiEye, guiLook, up);
	camera->RotateU(camRotU);
	camera->RotateV(camRotV);
	camera->RotateW(camRotW);
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (parser == NULL) {
		return;
	}

	if (pixels == NULL) {
		return;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(pixelWidth, pixelHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glutSwapBuffers();
}

void onExit()
{
	delete cube;
	delete cylinder;
	delete cone;
	delete sphere;
	delete camera;
	if (parser != NULL) {
		delete parser;
	}
	if (pixels != NULL) {
		delete pixels;
	}
}

/**************************************** main() ********************/

int main(int argc, char* argv[])
{
	atexit(onExit);

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	main_window = glutCreateWindow("COMP 175 Assignment 4");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI* glui = GLUI_Master.create_glui("GLUI");

	 // filenameTextField = new GLUI_EditText( glui, "Filename:", filenamePath);
	 // filenameTextField->set_w(300);
	glui->add_button("Load", 0, callback_load);
	glui->add_button("Start!", 0, callback_start);
	glui->add_checkbox("Isect Only", &isectOnly);
	
	GLUI_Panel *camera_panel = glui->add_panel("Camera");
	(new GLUI_Spinner(camera_panel, "RotateV:", &camRotV))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateU:", &camRotU))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateW:", &camRotW))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "Angle:", &viewAngle))
		->set_int_limits(1, 179);

	glui->add_column_to_panel(camera_panel, true);

	GLUI_Spinner* eyex_widget = glui->add_spinner_to_panel(camera_panel, "EyeX:", GLUI_SPINNER_FLOAT, &eyeX);
	eyex_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyey_widget = glui->add_spinner_to_panel(camera_panel, "EyeY:", GLUI_SPINNER_FLOAT, &eyeY);
	eyey_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyez_widget = glui->add_spinner_to_panel(camera_panel, "EyeZ:", GLUI_SPINNER_FLOAT, &eyeZ);
	eyez_widget->set_float_limits(-10, 10);

	GLUI_Spinner* lookx_widget = glui->add_spinner_to_panel(camera_panel, "LookX:", GLUI_SPINNER_FLOAT, &lookX);
	lookx_widget->set_float_limits(-10, 10);
	GLUI_Spinner* looky_widget = glui->add_spinner_to_panel(camera_panel, "LookY:", GLUI_SPINNER_FLOAT, &lookY);
	looky_widget->set_float_limits(-10, 10);
	GLUI_Spinner* lookz_widget = glui->add_spinner_to_panel(camera_panel, "LookZ:", GLUI_SPINNER_FLOAT, &lookZ);
	lookz_widget->set_float_limits(-10, 10);

	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();

	return EXIT_SUCCESS;
}


