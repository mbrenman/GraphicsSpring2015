#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"
#include <iostream>

const int NUM_FACES = 6;

enum RECT_SIDES {
	FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM, NUM_SIDES
};

class Cube : public Shape {
public:
	struct point_info {
		Point p;
		Vector normal;
	
		point_info(const Point &_p, const Vector &_norm) {
			p = _p;
			normal = _norm;
		};
	};

	Cube() {};
	~Cube() {};

	void draw() {
		for (int i = 0; i < NUM_FACES; i++) {
			drawFace(faces[i]);
		}
		draw_normal = false;
	};

	void recompute() {
		std::cout << "redrawing!" << std::endl;
		//clear Faces
		for (int i = 0; i < NUM_FACES; i++) {
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

		computeFace(Point(0.5, 0.5, 0.5),
			Vector(0.0, 0.0, -1.0 / m_segmentsY),
			Vector(-1.0 / m_segmentsX, 0.0, 0.0), faces[TOP]);

		computeFace(Point(0.5, -0.5, -0.5),
			Vector(0.0, 0.0, 1.0 / m_segmentsY),
			Vector(-1.0 / m_segmentsX, 0.0, 0.0), faces[BOTTOM]);
	
		for (int i = 0; i < NUM_FACES; i++) {
			std::cout << "face " << i << " size: " << faces[i].size() << std::endl;
		}
	};

	void drawNormal() {

	};

private:

	void drawFace(std::vector<point_info> &face) {
		for (int i = 0; i < m_segmentsY; i++) {
			glBegin(GL_TRIANGLE_STRIP);
			for (int j = 0; j < 2 * (m_segmentsX + 1); j++) {
				//std::cout << "size: " << face.size() << std::endl;
				//std::cout << (i * 2 * m_segmentsX) + j << std::endl;
				drawPoint(face.at((i * 2 * (m_segmentsX + 1)) + j));
			}
			glEnd();
		}
	}

	void drawPoint(const point_info &p_info) {
		Point p = p_info.p;
		glVertex3f(p.at(0), p.at(1), p.at(2));
	};

	void computeFace(Point startp, Vector vert_vec, Vector horiz_vec, std::vector<point_info> &face) {
		for (int i = 0; i < m_segmentsY; i++) {
			computeStrip(startp, vert_vec, horiz_vec, face);
			startp = startp + vert_vec;
		}
	};

	void computeStrip(Point startp, Vector vert_vec, Vector horiz_vec, std::vector<point_info> &face){
		Point p = startp;
		Vector normal = cross(horiz_vec, vert_vec);
		for (int i = 0; i <= m_segmentsX; i++) {
			face.push_back(point_info(p, normal));
			p = p + vert_vec;
			face.push_back(point_info(p, normal));
			p = p - vert_vec;
			p = p + horiz_vec;
		}
	};

	std::vector<point_info> faces[NUM_FACES];
};

#endif
