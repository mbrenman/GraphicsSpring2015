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
	
		for (int i = 0; i < NUM_SIDES; i++) {
			std::cout << "face " << i << " size: " << faces[i].size() << std::endl;
		}
	};

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

	std::vector<point_info> faces[NUM_SIDES];
};

#endif
