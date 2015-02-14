#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"


class Cylinder : public Shape {
public:
	Cylinder() {};
	~Cylinder() {};

	void draw() {
		for (int i = 0; i < body.size(); i++) {
			drawFace(body.at(i), false);
		}
		drawCap(caps[TOP], false);
		drawCap(caps[BOTTOM], false);
	};

	void drawNormal() {
		for (int i = 0; i < body.size(); i++) {
			drawFace(body.at(i), true);
		}
		drawCap(caps[TOP], true);
		drawCap(caps[BOTTOM], true);
	};

	void recompute() {
		std::cout << "redrawing!" << std::endl;
		//clear Faces
		for (int i = 0; i < body.size(); i++) {
			body[i].clear();
		}
		body.clear();

		m_rmatrix = rot_mat(Vector(0.0, 1.0, 0.0), -(2 * PI) / m_segmentsX);

		Point startp(0.5, -0.5, 0.0);
		Point nextp(0.5*cos(2 * PI / m_segmentsX), -0.5, 0.5*sin(2 * PI / m_segmentsX));
		computeBody(startp,
			nextp - startp,
			Vector(0.0, 1.0 / m_segmentsY, 0.0), body);

		computeCap(Point(0.0, -0.5, 0.0), startp, nextp, caps[BOTTOM], Vector(0.0, -1.0, 0.0));
		computeCap(Point(0.0, 0.5, 0.0), startp + Vector(0.0,1.0,0.0),
			nextp + Vector(0.0,1.0,0.0), caps[TOP], Vector(0.0, 1.0, 0.0));
	}
private:
	enum CYL_CAPS { TOP, BOTTOM, NUM_CAPS };

	void drawCap(std::vector<point_info> &cap, bool drawingNormals){
		if (drawingNormals) {
			glBegin(GL_LINES);
			for (int i = 0; i < cap.size(); i++){
				drawNormalAtVertex(cap.at(i));
			}
			glEnd();
		}
		else {
			glBegin(GL_TRIANGLE_FAN);
			for (int i = 0; i < cap.size(); i++){
				drawPoint(cap.at(i));
			}
			glEnd();
		}
	}

	void drawFace(std::vector<point_info> &face, bool drawingNormals) {
		for (int i = 0; i < m_segmentsX; i++) {
			if (drawingNormals) {
				glBegin(GL_LINES);
			}
			else {
				glBegin(GL_TRIANGLE_STRIP);
			}
			for (int j = 0; j < 2 * (m_segmentsY + 1); j++) {
				//std::cout << "size: " << face.size() << std::endl;
				//std::cout << (i * 2 * m_segmentsX) + j << std::endl;
				if (drawingNormals) {
					drawNormalAtVertex(face.at(j));
				}
				else {
					drawPoint(face.at(j));
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
