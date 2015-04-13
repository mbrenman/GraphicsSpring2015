#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"
#include <iostream>

class Cube : public Shape {
public:
	enum RECT_SIDES {
		FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM, NUM_SIDES
	};

	Cube() {};
	~Cube() {};

	void draw() {
		for (int i = 0; i < NUM_SIDES; i++) {
			drawPart(faces[i], GL_TRIANGLE_STRIP, false);
		}
	};

	void drawNormal() {
		for (int i = 0; i < NUM_SIDES; i++) {
			drawPart(faces[i], GL_TRIANGLE_STRIP, true);
		}
	};

	void recompute() {
		std::cout << "redrawing!" << std::endl;
		//clear Faces
		for (int i = 0; i < NUM_SIDES; i++) {
			faces[i].clear();
		}

		computeFace(Point(-0.5, -0.5, 0.5),
			Vector(0.0, 1.0 / m_segmentsY, 0.0),
			Vector(1.0 / m_segmentsX, 0.0, 0.0), faces[FRONT]);

		computeFace(Point(0.5, -0.5, -0.5),
			Vector(0.0, 1.0 / m_segmentsY, 0.0),
			Vector(-1.0 / m_segmentsX, 0.0, 0.0), faces[BACK]);

		computeFace(Point(-0.5, -0.5, -0.5),
			Vector(0.0, 1.0 / m_segmentsY, 0.0),
			Vector(0.0, 0.0, 1.0 / m_segmentsX), faces[LEFT]);

		computeFace(Point(0.5, -0.5, 0.5),
			Vector(0.0, 1.0 / m_segmentsY, 0.0),
			Vector(0.0, 0.0, -1.0 / m_segmentsX), faces[RIGHT]);

		computeFace(Point(0.5, -0.5, 0.5),
			Vector(0.0, 0.0, -1.0 / m_segmentsY),
			Vector(-1.0 / m_segmentsX, 0.0, 0.0), faces[BOTTOM]);

		computeFace(Point(0.5, 0.5, -0.5),
			Vector(0.0, 0.0, 1.0 / m_segmentsY),
			Vector(-1.0 / m_segmentsX, 0.0, 0.0), faces[TOP]);
	};

	double Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix) {
		eyePointP = invert(transformMatrix) * eyePointP;
		rayV = invert(transformMatrix) * rayV;
		double min_t = -1;
		double t;
		
		// //plane (0.5,0,0)
		// if (rayV.at(0) != 0) {
		// 	t = (0.5 - eyePointP.at(0)) / rayV.at(0);
		// 	min_t = ((t < min_t) || (min_t < 0)) && (t > 0) && testBounds(eyePointP, rayV, t) ? t : min_t;
		// }

		// //plane (-0.5,0,0)
		// if (rayV.at(0) != 0) {
		// 	t = (-0.5 - eyePointP.at(0)) / rayV.at(0);
		// 	min_t = ((t < min_t) || (min_t < 0)) && (t > 0) && testBounds(eyePointP, rayV, t) ? t : min_t;
		// }
		

		// //plane (0,0.5,0)
		// if (rayV.at(1) != 0) {
		// 	t = (0.5 - eyePointP.at(1)) / rayV.at(1);
		// 	min_t = ((t < min_t) || (min_t < 0)) && (t > 0) && testBounds(eyePointP, rayV, t) ? t : min_t;
		// }

		// //plane (0,-0.5,0)
		// if (rayV.at(1) != 0) {
		// 	t = (-0.5 - eyePointP.at(1)) / rayV.at(1);
		// 	min_t = ((t < min_t) || (min_t < 0)) && (t > 0) && testBounds(eyePointP, rayV, t) ? t : min_t;
		// }

		
		//plane (0,0,0.5)
		if (rayV.at(2) != 0) {
			t = (0.5 - eyePointP.at(2)) / rayV.at(2);
			min_t = ((t < min_t) || (min_t < 0)) && (t > 0) && testBounds(eyePointP, rayV, t) ? t : min_t;
		}

		//plane (0,0,-0.5)
		// if (rayV.at(2) != 0) {
		// 	t = (-0.5 - eyePointP.at(2)) / rayV.at(2);
		// 	min_t = ((t < min_t) || (min_t < 0)) && (t > 0) && testBounds(eyePointP, rayV, t) ? t : min_t;
		// }
		

		return testBounds(eyePointP, rayV, min_t) ? min_t : -1;
	}	

	Vector findIsectNormal(Point eyePointP, Vector rayV, double dist, Matrix transformMatrix) {
		eyePointP = invert(transformMatrix) * eyePointP;
		rayV = invert(transformMatrix) * rayV;
		double min_t = -1;
		double t;

		double epsilon = 0.000000001;
		
		return Vector(0, 0, 1);

		//plane (0.5,0,0)
		if (rayV.at(0) != 0) {
			t = (0.5 - eyePointP.at(0)) / rayV.at(0);
			min_t = ((t < min_t) || (min_t < 0)) && (t > 0) && testBounds(eyePointP, rayV, t) ? t : min_t;
		}

		if ((min_t != -1) && abs(dist - min_t) < epsilon) {
			return Vector(1, 0, 0);
		}

		//plane (-0.5,0,0)
		if (rayV.at(0) != 0) {
			t = (-0.5 - eyePointP.at(0)) / rayV.at(0);
			min_t = ((t < min_t) || (min_t < 0)) && (t > 0) && testBounds(eyePointP, rayV, t) ? t : min_t;
		}
		
		if ((min_t != -1) && abs(dist - min_t) < epsilon) {
			return Vector(-1, 0, 0);
		}

		//plane (0,0.5,0)
		if (rayV.at(1) != 0) {
			t = (0.5 - eyePointP.at(1)) / rayV.at(1);
			min_t = ((t < min_t) || (min_t < 0)) && (t > 0) && testBounds(eyePointP, rayV, t) ? t : min_t;
		}

		if ((min_t != -1) && abs(dist - min_t) < epsilon) {
			return Vector(0, 1, 0);
		}

		//plane (0,-0.5,0)
		if (rayV.at(1) != 0) {
			t = (-0.5 - eyePointP.at(1)) / rayV.at(1);
			min_t = ((t < min_t) || (min_t < 0)) && (t > 0) && testBounds(eyePointP, rayV, t) ? t : min_t;
		}

		if ((min_t != -1) && abs(dist - min_t) < epsilon) {
			return Vector(0, -1, 0);
		}
		
		//plane (0,0,0.5)
		if (rayV.at(2) != 0) {
			t = (0.5 - eyePointP.at(2)) / rayV.at(2);
			min_t = ((t < min_t) || (min_t < 0)) && (t > 0) && testBounds(eyePointP, rayV, t) ? t : min_t;
		}

		if ((min_t != -1) && abs(dist - min_t) < epsilon) {
			return Vector(0, 0, 1);
		}

		//plane (0,0,-0.5)
		if (rayV.at(2) != 0) {
			t = (-0.5 - eyePointP.at(2)) / rayV.at(2);
			min_t = ((t < min_t) || (min_t < 0)) && (t > 0) && testBounds(eyePointP, rayV, t) ? t : min_t;
		}
		
		if ((min_t != -1) && abs(dist - min_t) < epsilon) {
			return Vector(0, 0, 1);
		}

		return Vector();
	}

private:
	void computeFace(Point startp, Vector vert_vec, Vector horiz_vec, std::vector<point_info> &face) {
		for (int i = 0; i < m_segmentsY; i++) {
			computeStrip(startp, vert_vec, horiz_vec, face);
			startp = startp + vert_vec;
		}
	};

	void computeStrip(Point startp, Vector vert_vec, Vector horiz_vec, std::vector<point_info> &face){
		Point p = startp;
		Vector normal = cross(horiz_vec, vert_vec);
		normal.normalize();
		for (int i = 0; i <= m_segmentsX; i++) {
			face.push_back(point_info(p, normal, false));
			p = p + vert_vec;
			face.push_back(point_info(p, normal, i == m_segmentsX ? true : false));
			p = p - vert_vec;
			p = p + horiz_vec;
		}
	};

	bool testBounds(Point eye, Vector ray, double t) {
		Point p = eye + t*ray;

		double epsilon = 0.000001;

		return (p.at(0) >= -0.5 - epsilon) && (p.at(0) <= 0.5 + epsilon) &&
			(p.at(1) >= -0.5 - epsilon) && (p.at(1) <= 0.5 + epsilon) &&
			(p.at(2) >= -0.5 - epsilon) && (p.at(2) <= 0.5 + epsilon) &&
			(t > 0);
	}

	std::vector<point_info> faces[NUM_SIDES];
};

#endif
