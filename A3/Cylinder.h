#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape {
public:
	Cylinder() {};
	~Cylinder() {};

	void draw() {
		for (int i = 0; i < body.size(); i++) {
			drawPart(body.at(i), GL_TRIANGLE_STRIP, false);
		}
		drawPart(caps[TOP], GL_TRIANGLE_FAN, false);
		drawPart(caps[BOTTOM], GL_TRIANGLE_FAN, false);
	};

	void drawNormal() {
		for (int i = 0; i < body.size(); i++) {
			drawPart(body.at(i), GL_TRIANGLE_STRIP, true);
		}
		drawPart(caps[TOP], GL_TRIANGLE_FAN, true);
		drawPart(caps[BOTTOM], GL_TRIANGLE_FAN, true);
	};

	void recompute() {
		std::cout << "redrawing!" << std::endl;
		//clear Faces
		for (int i = 0; i < body.size(); i++) {
			body[i].clear();
		}
		body.clear();

		Vector v = Vector(0.0, 1.0, 0.0);
		m_rmatrix = rot_mat(v, -(2 * PI) / m_segmentsX);

		Point startp(0.5, -0.5, 0.0);
		Point nextp(0.5*cos(2 * PI / m_segmentsX), -0.5, 0.5*sin(2 * PI / m_segmentsX));
		computeBody(startp, nextp - startp, Vector(0.0, 1.0 / m_segmentsY, 0.0), body);

		computeCap(Point(0.0, -0.5, 0.0), startp, nextp, caps[BOTTOM], Vector(0.0, -1.0, 0.0));
		computeCap(Point(0.0, 0.5, 0.0), startp + Vector(0.0, 1.0, 0.0),
			nextp + Vector(0.0, 1.0, 0.0), caps[TOP], Vector(0.0, 1.0, 0.0));
	}
private:
	enum CYL_CAPS { TOP, BOTTOM, NUM_CAPS };

	void computeBody(Point startp, Vector rot_vec, Vector tess_vec, std::vector< std::vector<point_info> > &body) {
		for (int i = 0; i < m_segmentsX; i++) {
			std::vector< point_info > face;
			computeStrip(startp, rot_vec, tess_vec, face);
			startp = startp + rot_vec;
			rot_vec = m_rmatrix * rot_vec;
			body.push_back(face);
		}
	};

	void computeStrip(Point startp, Vector vert_vec, Vector horiz_vec, std::vector<point_info> &face){
		Point p = startp;
		for (int i = 0; i <= m_segmentsY; i++) {
			face.push_back(point_info(p, cylBodyVector(p), false));
			p = p + vert_vec;
			face.push_back(point_info(p, cylBodyVector(p), i == m_segmentsY ? true : false));
			p = p - vert_vec;
			p = p + horiz_vec;
		}
	};

	Vector cylBodyVector(Point p) {
		Vector normal = p - Point(0, p.at(1), 0);
		normal.normalize();
		return normal;
	}

	void computeCap(Point centerpt, Point edgept1, Point edgept2, std::vector<point_info> &cap, Vector normal){
		normal.normalize();
		cap.clear();
		cap.push_back(point_info(centerpt, normal, false));
		Vector rot_vec = edgept2 - edgept1;
		for (int i = 0; i <= m_segmentsX; i++){
			cap.push_back(point_info(edgept1, normal, i == m_segmentsX ? true : false));
			edgept1 = edgept1 + rot_vec;
			rot_vec = m_rmatrix * rot_vec;
		}
	};

	std::vector< std::vector<point_info> > body;
	std::vector< point_info > caps[NUM_CAPS];
	Matrix m_rmatrix;
};
#endif
