/*  =================== File Information =================
  File Name: ply.cpp
  Description:
  Author: (You)

  Purpose:
  Examples:
  ===================================================== */
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <GL/glui.h>
#include "ply.h"
#include "geometry.h"
#include "entity.h"


using namespace std;
ifstream myfile;
string filePath;
int vertexCount;
int faceCount;
/*  ===============================================
	  Desc: Default constructor for a ply object
	  Precondition:
	  Postcondition:
	  =============================================== */
ply::ply(string _filePath){
	filePath = _filePath;
	vertexList = NULL;
	faceList = NULL;
	properties = 0;
	// Call helper function to load geometry
	loadGeometry();
}

/*  ===============================================
	  Desc: Destructor for a ply object
	  Precondition: Memory has been already allocated
	  Postcondition:
	  =============================================== */
ply::~ply(){
	// Delete the allocated faceList and vertexList arrays
	// and set both pointers to NULL
	delete[] vertexList;

	for (int i = 0; i < faceCount; i++) {
		delete[] faceList[i].vertexList;
	}

	delete[] faceList;
	vertexList = NULL;
	faceList = NULL;
}

/*  ===============================================
	  Desc: reloads the geometry for a 3D object
	  Precondition: Memory has been allocated and loaded, 
					_filePath is a .ply file.
	  Postcondition: Memory represents the ply file given by
					_filePath.  
	  =============================================== */
void ply::reload(string _filePath){
	filePath = _filePath;
	// reclaim memory allocated in each array
	delete[] vertexList;

	for (int i = 0; i < faceCount; i++) {
		delete[] faceList[i].vertexList;
	}

	delete[] faceList;
	// Set pointers to array
	vertexList = NULL;
	faceList = NULL;
	// Call our function again to load new vertex and face information.
	loadGeometry();
}


/*  ===============================================
	  Desc: Parses a file and loads vertices into vertexList and faceList.
	  Precondition:
	  Postcondition:
	  =============================================== */


void ply::loadGeometry(){

        /*
		   1.) Update any private or helper variables in the ply.h private section
		   2.) allocate memory for the vertexList
		   3.) Populate vertices
		   4.) allocate memory for the faceList
		   5.) Populate faceList
		*/


	// Parse header   
	myfile.open(filePath.c_str());
	if (myfile.is_open()) {

		// Read in an entire line
		string line;
		getline(myfile, line);
		cout << "Reading file: " << filePath << endl;
		int properties = -2; // because there are two extra lines of property info 
		bool readingHeader = true;
		while (getline(myfile, line) && readingHeader){
			char* copy = new char[(line.length() + 1)];
			strcpy(copy, line.c_str());
			char* currentWord = strtok(copy, " ");
			cout << line << endl;
			// ====== Parsing Header information ======
			// Read the size of the vertexList and faceList, property types and
			// number of properties from the header section of the ply file. 
			/* Example:
			element vertex 300
			element face 600
			property float x
			property float y
			property float z
			property float confidence
			property float intensity
			end_header
			*/
			//  would represent a ply file with 300 vertices, 600 faces,
			//  and 5 properties.  
			//  There are 7 possible properties: x, y, z, confidence, 
			//    intesity, nx, ny, and nz.       
			if (strcmp(currentWord, "element") == 0){
				currentWord = strtok(NULL, " ");
				if (strcmp(currentWord, "vertex") == 0){
					currentWord = strtok(NULL, " ");
					// Allocate memory for our vertices
					vertexCount = atoi(currentWord)- 1;
					vertexList = new vertex[vertexCount];

					// If the vertex list is not allocated (because of lack of memory) 
					// then the user is informed and the program exits with a code of 1
					if (vertexList == NULL){
						cout << "Cannot allocate memory for Vertex List: System out of memory" << endl;
						exit(1);
					}
				}

				// Executed if the element is faces instead of verteces.
				else if (strcmp(currentWord, "face") == 0){ // Set the face count
					currentWord = strtok(NULL, " ");
					// Allocate memory for our faces
					faceCount = atoi(currentWord);
					faceList = new face[faceCount];


					if (faceList == NULL){
						cout << "Cannot allocate memory for FaceList: System out of memory" << endl;
						exit(1);
					}
				}

				// If the delimeter is a unrecognized string then an error is
                // thrown and the program exits with a code of 1

				else{
					cout << currentWord << " is not a valid input variable" << endl;
					exit(1);
				}
			}

			else if (strcmp(currentWord, "property") == 0){ // Count the properties
				properties++;
			}
			else if (strcmp(currentWord, "end_header") == 0){
				cout << "found end_header" << endl;
				readingHeader = false;
			}
			delete(copy);
		}
		cout << "Reading Vertices" << endl;
		bool readingVertices = true;
		int verticesRead = 0;
		while (getline(myfile, line) && (verticesRead < vertexCount)){
			//cout << (vertexCount - verticesRead) << endl;
			char* copy = new char[line.length() + 1];
			strcpy(copy, line.c_str());
			char* currentWord = strtok(copy, " ");
			if (verticesRead >= vertexCount){
				readingVertices = false;
			}

			//
			if (verticesRead < vertexCount){
				if (properties >= 0){  // Read in the x vertex
					vertexList[verticesRead].x = atof(currentWord);
				}
				if (properties >= 1){   // Read in the y vertex
					currentWord = strtok(NULL, " ");
					vertexList[verticesRead].y = atof(currentWord);
				}
				if (properties >= 2){  // Read in the z vertex
					currentWord = strtok(NULL, " ");
					vertexList[verticesRead].z = atof(currentWord);
				}
				if (properties >= 3){   //read in the confidence of a vertex
					currentWord = strtok(NULL, " ");
					vertexList[verticesRead].confidence = atof(currentWord);
				}
				if (properties >= 4){ // read in the intensity of a vertex
					currentWord = strtok(NULL, " ");
					try { vertexList[verticesRead].intensity = atof(currentWord); }
					catch (exception e){
						cout << line << endl; 
						throw e;
					}
				}
				if (properties >= 5){
					currentWord = strtok(NULL, " ");
					vertexList[verticesRead].nx = atof(currentWord);
				}
				if (properties >= 6){
					currentWord = strtok(NULL, " ");
					vertexList[verticesRead].ny = atof(currentWord);
				}
				if (properties >= 7){
					currentWord = strtok(NULL, " ");
					vertexList[verticesRead].nz = atof(currentWord);
				}
				verticesRead++;
			}
			delete(copy);
		}
		cout << "Reading Faces" << endl;
		bool readingFaces = true;
		int facesRead = 0;
		while (getline(myfile, line) && readingFaces){
			//cout << line << endl;
			char* copy = new char[line.length() + 1];
			strcpy(copy, line.c_str());
			char* currentWord = strtok(copy, " ");
			if (facesRead >= faceCount){
				readingFaces = false;
			}

			if (facesRead < faceCount){
				// first number is how many faces to read in
				faceList[facesRead].vertexCount = atoi(currentWord);
				// Allocate the vertex list with memory 
				faceList[facesRead].vertexList = new int[faceList[facesRead].vertexCount];
				// Then we store the vertices until our delimeter is null
				int count = 0;
				while (count < faceList[facesRead].vertexCount){
					currentWord = strtok(NULL, " ");
					faceList[facesRead].vertexList[count] = atoi(currentWord);
					count++;
				}
				facesRead++;
			}
			delete(copy);
		}
		myfile.close();
		scaleAndCenter();
	}
	else{
		cout << "Unable to open file: " << filePath << endl;
	}

}

/*  ===============================================
Desc: Moves all the geometry so that the object is centered at 0, 0, 0 and 
    scaled to be between 0.5 and -0.5 
Precondition: after all the vetices and faces have been loaded in
Postcondition:
=============================================== */
void ply::scaleAndCenter() {
  int i;
  float max = 0.0f;


  //fill in




  //sanity check. The printf should say that the max is 0.5
  for (i = 0; i < vertexCount; i++){
    if (max < fabs(vertexList[i].x)) max = fabs(vertexList[i].x);
    if (max < fabs(vertexList[i].y)) max = fabs(vertexList[i].y);
    if (max < fabs(vertexList[i].z)) max = fabs(vertexList[i].z);
  }
  printf("max vertex value: %f\n", max);
}

/*  ===============================================
Desc:   Finds the normal to three vertices(1 face making up a triangle)
Precondition:
Postcondition:  Calls 'glNormal3f()' which takes in 3 parameters
=============================================== */
void ply::setNormal(float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3) {


	float v1x, v1y, v1z;
	float v2x, v2y, v2z;
	float cx, cy, cz;

	//find vector between x2 and x1
	v1x = x1 - x2;
	v1y = y1 - y2;
	v1z = z1 - z2;

	//find vector between x3 and x2
	v2x = x2 - x3;
	v2y = y2 - y3;
	v2z = z2 - z3;

	//cross product v1xv2

	cx = v1y * v2z - v1z * v2y;
	cy = v1z * v2x - v1x * v2z;
	cz = v1x * v2y - v1y * v2x;

	//normalize

	float length = sqrt(cx * cx + cy * cy + cz * cz);
	cx = cx / length;
	cy = cy / length;
	cz = cz / length;

	glNormal3f(cx, cy, cz);
}

/*  ===============================================
	  Desc: Draws a filled 3D object
	  Precondition:
	  Postcondition:
	  Error Condition: If we haven't allocated memory for our
	  faceList or vertexList then do not attempt to render.
	  =============================================== */
void ply::render(){
	// Given a triangle so that you see something on the scene
    
    //----- The code below draws only a grey triangle -----//
	float x1, y1, z1;
	float x2, y2, z2;
	float x3, y3, z3;

	z1 = z2 = z3 = 0;
	x1 = -0.5;
	x2 = 0.5;
	x3 = 0;
	y1 = -0.5;
	y2 = -0.5;
	y3 = 0.5;

	/*glBegin(GL_TRIANGLES); // sets the shapes that openGL draws and determines
                           // the number of vertices that are necessary
	setNormal(x1, y1, z1, x2, y2, z2, x3, y3, z3); // makes sure that each 
                                                   // vertex is correctly 
                                                   // scaled
	glVertex3f(x1, y1, z1);  // set the three vertices for the triangle
	glVertex3f(x2, y2, z2);  // the direction of the front face depends 
	glVertex3f(x3, y3, z3);  // on the order in which you put the vertices
	glEnd();*/

	/* Basic idea
		Loop through each face, set the normal, output correct vertices

		Items of interest/functions that will come in handy:
		setNormal(...) 
		glBegin(GL_TRIANGLES); // this begins a new triangle to draw
		glVertex3f();
		faceCount
		vertexCount
		faceList
		vertexList

	*/

	
	if(vertexList==NULL || faceList==NULL){
	return;
	}

	/*
	start here..

	*/
	glBegin(GL_TRIANGLES); // sets the shapes that openGL draws and determines
	
	for (int i = 0; i < faceCount; i++) {
		face currface = faceList[i];
		if (currface.vertexList != NULL) {
			for (int j = 0; j < 3; j++){
				int index = currface.vertexList[j];
				vertex v = vertexList[index];
				//setNormal(x1, y1, z1, x2, y2, z2, x3, y3, z3); // makes sure that each 
				glVertex3f(v.x, v.y, v.z);  // set the three vertices for the triangle
			}
		}
	}
	glEnd();
}

/*  ===============================================
  Desc: Draws the wireframe(edges) of a 3D object.
  We could alternatively use 'glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);'

  Note, having two rendering functions is too much, but think
  about how you could modularize this, or other interseting
  ways of rendering. Could you render an object as points?
  What about rendering quads versus triangles, or using triangle
  strips if you know something about the data?

  Note that here we also do not set the normals for lighting. This way our
  geometry could load faster, perhaps different functions could
  have different special effects.

  Precondition:
  Postcondition:
  =============================================== */
void ply::renderWireFrame(){
	/* This is optional */
}


/*  ===============================================
	  Desc: Prints some statistics about the file you have read in
	  This is useful for debugging information to see if we parse our file 
      correctly.

	  Precondition:
	  Postcondition:
	  =============================================== */
void ply::printAttributes(){
	cout << "==== ply Mesh Attributes=====" << endl;
	cout << "vertex count:" << vertexCount << endl;
	cout << "face count:" << faceCount << endl;
	cout << "properties:" << properties << endl;
}

/*  ===============================================
	  Desc: Iterate through our array and print out each vertex.

	  Precondition:
	  Postcondition:
	  =============================================== */
void ply::printVertexList(){
	if (vertexList == NULL){
		return;
	}
	else{
		for (int i = 0; i < vertexCount; i++){
			cout << vertexList[i].x << "," << vertexList[i].y;
            cout << "," << vertexList[i].z << endl;
		}
	}
}

/*  ===============================================
	  Desc: Iterate through our array and print out each face.

	  Precondition:
	  Postcondition:
	  =============================================== */
void ply::printFaceList(){
	if (faceList == NULL){
		return;
	}
	else{
		// For each of our faces
		for (int i = 0; i < faceCount; i++){
			// Get the vertices that make up each face from the face list
			for (int j = 0; j < faceList[i].vertexCount; j++){
				// Print out the vertex
				int index = faceList[i].vertexList[j];
				cout << vertexList[index].x << "," << vertexList[index].y;
                cout << "," << vertexList[index].z << endl;
			}
		}
	}
}
