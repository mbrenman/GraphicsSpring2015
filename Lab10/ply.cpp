/*  =================== File Information =================
  File Name: ply.cpp
  Description:
  Author: Michael Shah
  Last Modified: 4/24/14

  Purpose:
  Examples:
  ===================================================== */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <math.h>
//#include "GLee.h"
#include <GL/glew.h>
#include <GL/glui.h>
#include "ply.h"
#include "geometry.h"


using namespace std;

/*  ===============================================
      Desc: Default constructor for a ply object
      Precondition:
      Postcondition:
    =============================================== */ 
ply::ply(string _filePath){
	filePath = _filePath;
	vertexList = NULL;
	faceList = NULL;
	vertex_vao = NULL;
	indicies_vao = NULL;
	normals_vao = NULL;

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
  // Delete the allocated arrays
	delete[] vertexList;

  for (int i = 0; i < faceCount; i++) {
    delete [] faceList[i].vertexList;
  }

	delete[] faceList;
  // Set pointers to NULL
  vertexList = NULL;
  faceList = NULL;

  if(vertex_vao != NULL){
      delete[] vertex_vao;
      vertex_vao = NULL;
  }

  if(indicies_vao != NULL){
      delete[] indicies_vao;
      indicies_vao = NULL;
  }

  if (normals_vao != NULL){
	  delete[] normals_vao;
	  normals_vao = NULL;
  }

  // Destroy our vertex buffer object
  glDeleteBuffers(1, &vertexVBO_id);
  glDeleteBuffers(1, &indicesVBO_id);
  glDeleteBuffers(1, &normalVBO_id);
}


/*  ===============================================
  Desc: Simple function that builds all of the arrays to be used
        in vertex buffer objects.


  Precondition:
  Postcondition:
=============================================== */  
void ply::buildArrays(unsigned int programID){
    // allocate memory for our arrays
    vertex_vao = new GLfloat[vertexCount*3];
    if(vertex_vao==NULL){
      cout << "Ran out of memory(vertex_vao)!" << endl;
      return;
    }
    
    indicies_vao = new GLuint[faceCount*3];
    if(indicies_vao==NULL){
      cout << "Ran out of memory(indicies_vao)!" << endl;
      return;
    }
    
	normals_vao = new GLfloat[vertexCount * 3];
	if (normals_vao == NULL){
		cout << "Ran out of memory(normals_vao)!" << endl;
		return;
	}
	for (int i = 0; i < vertexCount * 3; i++) {
		normals_vao[i] = 0.0;
	}

	int* numNormals = new int[vertexCount];
	if (numNormals == NULL){
		cout << "Ran out of memory(normal Counter)!" << endl;
		return;
	}
	for (int i = 0; i < vertexCount; i++) {
		numNormals[i] = 0;
	}


    // Compress all of the vertices into a simple array
    if(vertexList==NULL){
        return;
    }else{
        for(int i = 0; i < vertexCount; i++){
              vertex_vao[i*3+0] = vertexList[i].x;
              vertex_vao[i*3+1] = vertexList[i].y;
              vertex_vao[i*3+2] = vertexList[i].z;
        }
    }

    // Compress everything into one array of indices
    unsigned int k = 0;
    if(faceList==NULL){
      return;
    }else{
      // For each of our faces
      for(int i = 0; i < faceCount; i++){
        // Get the vertices that make up each face from the face list
        int index0 = faceList[i].vertexList[0];
        int index1 = faceList[i].vertexList[1];
        int index2 = faceList[i].vertexList[2];

        indicies_vao[k] = index0;
        indicies_vao[k+1] = index1;
        indicies_vao[k+2] = index2;

		k += 3;
      }
    }

	//this part is for building the vertex normals -- it looks complicated because 
	// we are computing the "smooth" normals of a vertex, which is defined as the "average"
	// of all the normals at a vertex. 
	//That is, let's say that a vertex is at the intersection of 5 triangles, each triangle
	// will have its own normal (computed using the cross product).
	//To determine the "smooth" normal at that vertex, we add up all 5 triangle normals and then
	// divide by 5.
	//In order to do this, there are two steps:
	//  1. we sum up all the normals. 
	//  2. While we are doing that, we need to keep a counter for the number of normals at a vertex
	for (int i = 0; i < faceCount * 3; i = i + 3) {
		int index0 = indicies_vao[i];
		int index1 = indicies_vao[i + 1];
		int index2 = indicies_vao[i + 2];

		float outputx, outputy, outputz;
		// using the setNormal function from below we normalize the vectors
		computeNormal(
			vertex_vao[index0 * 3 + 0], vertex_vao[index0 * 3 + 1], vertex_vao[index0 * 3 + 2],
			vertex_vao[index1 * 3 + 0], vertex_vao[index1 * 3 + 1], vertex_vao[index1 * 3 + 2],
			vertex_vao[index2 * 3 + 0], vertex_vao[index2 * 3 + 1], vertex_vao[index2 * 3 + 2],
			&outputx, &outputy, &outputz);

		numNormals[index0]++;
		numNormals[index1]++;
		numNormals[index2]++;

		normals_vao[index0 * 3 + 0] += outputx;
		normals_vao[index0 * 3 + 1] += outputy;
		normals_vao[index0 * 3 + 2] += outputz;

		normals_vao[index1 * 3 + 0] += outputx;
		normals_vao[index1 * 3 + 1] += outputy;
		normals_vao[index1 * 3 + 2] += outputz;

		normals_vao[index2 * 3 + 0] += outputx;
		normals_vao[index2 * 3 + 1] += outputy;
		normals_vao[index2 * 3 + 2] += outputz;
	}
	for (int i = 0; i < vertexCount; i++) {
		normals_vao[i * 3 + 0] = normals_vao[i * 3 + 0] / (float)(numNormals[i]);
		normals_vao[i * 3 + 1] = normals_vao[i * 3 + 1] / (float)(numNormals[i]);
		normals_vao[i * 3 + 2] = normals_vao[i * 3 + 2] / (float)(numNormals[i]);
	}


	delete[] numNormals;

   
	// Use a Vertex Array Object -- think of this as a single ID that sums up all the following VBOs
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    // Generate a buffer that will be sent to the video memory
    // For each ply object, it will get its own vertex buffer object, but it
    // would be much more efficient to put everything into one Vertex Buffer Object and send it over
    // only once to video memory. 

	// Note: If this seg faults, then Glee or Glew (however OpenGL 2.0 extensions are mangaged)
    // has not yet been initialized.
	//tell openGL to generate a new VBO object
	glGenBuffers(1, &vertexVBO_id);
    // Once we know how many buffers to generate, then hook up the buffer to the vertexVBO_id.
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO_id);
    // Now we finally copy data into the buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertexCount * 3, vertex_vao, GL_STATIC_DRAW);


	//tell openGL to generate a new VBO object
	glGenBuffers(1, &indicesVBO_id);
	// Transfer the data from indices to a VBO indicesVBO_id
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO_id);
	// Copy data into the buffer object. Note the keyword difference here -- GL_ELEMENT_ARRAY_BUFFER
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*faceCount * 3, indicies_vao, GL_STATIC_DRAW);


	// Get the location of the attributes that enters in the vertex shader
	GLint position_attribute = glGetAttribLocation(programID, "position");
	// Specify how the data for position can be accessed
	glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Enable the attribute
	glEnableVertexAttribArray(position_attribute);






	//TODO: Repeat the process as building a vertexVBO, but this time for the normals

	//*****************This is for you to fill in!!!  ****************************//





    cout << "Created vbo successfully" << endl;
}

void ply::renderVBO() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, faceCount*3, GL_UNSIGNED_INT, 0);
	
	

	//************The following code is for using VBO but not shaders ****************//
	//glDrawArrays(GL_TRIANGLES, 0, faceCount);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, (char*)NULL + 0);
	//glDrawElements(GL_TRIANGLES, faceCount*3, GL_UNSIGNED_INT, 0);

	//glDisableClientState(GL_VERTEX_ARRAY);



	//************The following code is for using the old fixed pipeline (so called immediate mode) ****************//
	//int i;

	//glBegin(GL_TRIANGLES);

	//for (i = 0; i < faceCount*3; i=i+3) {
	//	int index0 = indicies_vao[i];
	//	int index1 = indicies_vao[i+1];
	//	int index2 = indicies_vao[i+2];

	//	glNormal3f(normals_vao[index0 * 3], normals_vao[index0 * 3 + 1], normals_vao[index0 * 3 + 2]);
	//	glVertex3f(vertex_vao[index0*3], vertex_vao[index0*3+1], vertex_vao[index0*3+2]);

	//	glNormal3f(normals_vao[index1 * 3], normals_vao[index1 * 3 + 1], normals_vao[index1 * 3 + 2]);
	//	glVertex3f(vertex_vao[index1 * 3], vertex_vao[index1 * 3 + 1], vertex_vao[index1 * 3 + 2]);

	//	glNormal3f(normals_vao[index2 * 3], normals_vao[index2 * 3 + 1], normals_vao[index2 * 3 + 2]);
	//	glVertex3f(vertex_vao[index2 * 3], vertex_vao[index2 * 3 + 1], vertex_vao[index2 * 3 + 2]);
	//}

	//glEnd();
}


/*  ===============================================
      Desc: Prints some statistics about the file you have read in
      This is useful for debugging information to see if we parse our file correctly.

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
  	if(vertexList==NULL){
  		  return;
  	}else{
    		for(int i = 0; i < vertexCount; i++){
    			   cout << vertexList[i].x << "," << vertexList[i].y << "," << vertexList[i].z << endl;
    		}
  	}
}

/*  ===============================================
      Desc: Iterate through our array and print out each face.

      Precondition:
      Postcondition:  
    =============================================== */ 
void ply::printFaceList(){
	if(faceList==NULL){
		return;
	}else{
		// For each of our faces
		for(int i = 0; i < faceCount; i++){
			// Get the vertices that make up each face from the face list
			for(int j = 0; j < faceList[i].vertexCount; j++){
				// Print out the vertex
				int index = faceList[i].vertexList[j];
				cout << vertexList[index].x << "," << vertexList[index].y << "," << vertexList[index].z << endl;
			}
		}
	}
}


/*  ===============================================
Desc: Moves all the geometry so that the object is centered at 0, 0, 0 and scaled to be between 0.5 and -0.5
Precondition: after all the vetices and faces have been loaded in
Postcondition:
=============================================== */
void ply::scaleAndCenter() {

	//first, center the object

	//find the center point
	float cx, cy, cz;
	cx = 0;
	cy = 0;
	cz = 0;
	for (int i = 0; i < vertexCount; i++) {
		cx += vertexList[i].x;
		cy += vertexList[i].y;
		cz += vertexList[i].z;
	}
	cx = cx / (float)vertexCount;
	cy = cy / (float)vertexCount;
	cz = cz / (float)vertexCount;

	//now shift all the points so that cx, cy, cz is at the center
	for (int i = 0; i < vertexCount; i++) {
		vertexList[i].x -= cx;
		vertexList[i].y -= cy;
		vertexList[i].z -= cz;
	}

	//now find the largest value to scale it down
	float max = 0;
	for (int i = 0; i < vertexCount; i++) {
		if (fabs(vertexList[i].x) > max) {
			max = fabs(vertexList[i].x);
		}
		if (fabs(vertexList[i].y) > max) {
			max = fabs(vertexList[i].y);
		}
		if (fabs(vertexList[i].z) > max) {
			max = fabs(vertexList[i].z);
		}
	}

	//now scale everything, but double max so that everything is between 0.5 and -0.5
	max *= 2;
	for (int i = 0; i < vertexCount; i++) {
		vertexList[i].x /= max;
		vertexList[i].y /= max;
		vertexList[i].z /= max;
	}
}

/*  ===============================================
Desc:   Finds the normal to three vertices(1 face making up a triangle)
Precondition:
Postcondition:  returns the normals as outputx, outputy, and outputz
=============================================== */
void ply::computeNormal(float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3,
	float* outputx, float* outputy, float*outputz)
{

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

	*outputx = cx;
	*outputy = cy;
	*outputz = cz;

	//glNormal3f(cx, cy, cz);
}



/*  ===============================================
Desc: Parses a file and loads vertices into vertexList and faceList.
Precondition:
Postcondition:
=============================================== */
void ply::loadGeometry(){

	/*
	1. Parse the header
	2.) Update any private or helper variables in the ply.h private section
	3.) allocate memory for the vertexList
	3a.) Populate vertices
	4.) allocate memory for the faceList
	4a.) Populate faceList
	*/


	// Parse header   
	ifstream myfile(filePath.c_str());
	if (myfile.is_open()) {
		string line;
		bool readVertices = false;  // start reading data into our vertex list
		bool readFaces = false; // start reading data into our face list
		int verticesRead = 0;   // How many vertices we have read
		int facesRead = 0;      // How many faces have we read
		properties = -2;      // Starts at -1 because it appears 1 additional time in the file
		// Read in an entire line
		while (getline(myfile, line)) {
			// Split the line into space separated tokens
			char* delimeter_pointer;
			char* copy = new char[line.length() + 1];
			strcpy(copy, line.c_str());
			delimeter_pointer = strtok(copy, " ");

			while (delimeter_pointer != NULL){
				// ====== Parsing Header information ======
				if (strcmp(delimeter_pointer, "vertex") == 0){  // Set the vertex count
					delimeter_pointer = strtok(NULL, " ");
					vertexCount = atoi(delimeter_pointer);
					// Allocate memory for our vertices
					vertexList = new vertex[vertexCount];
					if (vertexList == NULL){
						cout << "Cannot allocate memory for Vertex List: System out of memory" << endl;
						exit(1);
					}
				}
				else if (strcmp(delimeter_pointer, "face") == 0){ // Set the face count
					delimeter_pointer = strtok(NULL, " ");
					faceCount = atoi(delimeter_pointer);
					// Allocate memory for our faces
					faceList = new face[faceCount];
					if (faceList == NULL){
						cout << "Cannot allocate memory for FaceList: System out of memory" << endl;
						exit(1);
					}
				}
				else if (strcmp(delimeter_pointer, "property") == 0){ // Count the properties
					properties++;
				}
				// ====== Parsing Header information ======

				// Switch state so we read in faces
				if (verticesRead >= vertexCount && readFaces == false && readVertices == true){
					readVertices = false;
					readFaces = true;
				}
				//
				if (readVertices == true && verticesRead < vertexCount){
					if (properties >= 0){  // Read in the x vertex
						vertexList[verticesRead].x = atof(delimeter_pointer);
					}
					if (properties >= 1){   // Read in the y vertex
						delimeter_pointer = strtok(NULL, " ");
						vertexList[verticesRead].y = atof(delimeter_pointer);
					}
					if (properties >= 2){  // Read in the z vertex
						delimeter_pointer = strtok(NULL, " ");
						vertexList[verticesRead].z = atof(delimeter_pointer);
					}
					if (properties >= 3){   //
						delimeter_pointer = strtok(NULL, " ");
						vertexList[verticesRead].confidence = atof(delimeter_pointer);
					}
					if (properties >= 4){
						delimeter_pointer = strtok(NULL, " ");
						vertexList[verticesRead].intensity = atof(delimeter_pointer);
					}
					if (properties >= 5){
						delimeter_pointer = strtok(NULL, " ");
						vertexList[verticesRead].nx = atof(delimeter_pointer);
					}
					if (properties >= 6){
						delimeter_pointer = strtok(NULL, " ");
						vertexList[verticesRead].ny = atof(delimeter_pointer);
					}
					if (properties >= 7){
						delimeter_pointer = strtok(NULL, " ");
						vertexList[verticesRead].nz = atof(delimeter_pointer);
					}
					verticesRead++;
				}
				else if (readFaces == true && facesRead < faceCount){
					// first number is how many faces to read in
					faceList[facesRead].vertexCount = atoi(delimeter_pointer);
					// Allocate the vertex list with memory
					faceList[facesRead].vertexList = new int[faceList[facesRead].vertexCount];
					// Then we store the vertices until our delimeter is null
					int count = 0;
					while (count < faceList[facesRead].vertexCount){
						delimeter_pointer = strtok(NULL, " ");
						faceList[facesRead].vertexList[count] = atoi(delimeter_pointer);
						count++;
					}
					facesRead++;
				}

				// Switch state so we immedietely start reading in vertices the next iteration
				if (strcmp(delimeter_pointer, "end_header") == 0){
					readVertices = true;
				}

				if (delimeter_pointer != NULL){
					delimeter_pointer = strtok(NULL, " ");
				}
			}
			delete copy;
		}
		myfile.close();

		scaleAndCenter();
	}
	else{
		cout << "Unable to open file: " << filePath << endl;
	}

	cout << "PLY Loaded" << endl;
	printAttributes();

}
