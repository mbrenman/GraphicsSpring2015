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
			drawFace(faces[i], false);
		}
	};

	void drawNormal() {
		for (int i = 0; i < NUM_SIDES; i++) {
			drawFace(faces[i], true);
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
	void drawFace(std::vector<point_info> &face, bool drawingNormals) {
		for (int i = 0; i < m_segmentsY; i++) {
			if (drawingNormals) {
				glBegin(GL_LINES);
			}
			else {
				glBegin(GL_TRIANGLE_STRIP);
			}
			for (int j = 0; j < 2 * (m_segmentsX + 1); j++) {
				//std::cout << "size: " << face.size() << std::endl;
				//std::cout << (i * 2 * m_segmentsX) + j << std::endl;
				if (drawingNormals) {
					drawNormalAtVertex(face.at((i * 2 * (m_segmentsX + 1)) + j));
				}
				else {
					drawPoint(face.at((i * 2 * (m_segmentsX + 1)) + j));
				}
			}
			glEnd();
		}
	}

	void drawPoint(const point_info &p_info) {
		Point p = p_info.p;
		Vector n = p_info.normal;
		glNormal3f(n.at(0), n.at(1), n.at(2));
		glVertex3f(p.at(0), p.at(1), p.at(2));
	};

	void drawNormalAtVertex(const point_info &p_info) {
		Point p = p_info.p;
		Vector norm = 0.1 * p_info.normal;
		glVertex3f(p.at(0), p.at(1), p.at(2));

		p = p + norm;
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
		normal.normalize();
		for (int i = 0; i <= m_segmentsX; i++) {
			face.push_back(point_info(p, normal));
			p = p + vert_vec;
			face.push_back(point_info(p, normal));
			p = p - vert_vec;
			p = p + horiz_vec;
		}
	};

	std::vector<point_info> faces[NUM_SIDES];
};

#endif
