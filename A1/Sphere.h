#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
	struct point_info {
		Point p;
		Vector normal;

		point_info(const Point &_p, const Vector &_norm) {
			p = _p;
			normal = _norm;
		};
	};

	Sphere() {};
	~Sphere() {};

	void draw() {
		for (int i = 0; i < body.size(); i++) {
			drawFace(body.at(i), false);
		}
	};

	void drawNormal() {
		for (int i = 0; i < body.size(); i++) {
			drawFace(body.at(i), true);
		}
	};

	void recompute() {
		std::cout << "redrawing!" << std::endl;
		//clear Faces
		for (int i = 0; i < body.size(); i++) {
			body[i].clear();
		}
		body.clear();

		m_hmatrix = rot_mat(Vector(0.0, 1.0, 0.0), -(2 * PI) / m_segmentsX);
		Point startp(0.5, 0.0, 0.0);
		Point nextp(0.5*cos(2 * PI / m_segmentsX), 0.0, 0.5*sin(2 * PI / m_segmentsX));
		
		computeBody(startp, nextp, body);
	}
private:
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
		Vector norm = p_info.normal;
		glVertex3f(p.at(0), p.at(1), p.at(2));

		p = p + norm;
		glVertex3f(p.at(0), p.at(1), p.at(2));
	};

	void computeBody(Point startp, Point nextp, std::vector< std::vector<point_info> > &body) {		
		for (int i = 0; i < m_segmentsX; i++) {
			std::vector< point_info > face;
			computeStrip(0.5, i * (2 * PI / m_segmentsX), face);
			body.push_back(face);
		}
	};

	void computeStrip(float r, float phi, std::vector<point_info> &face){
		for (int i = 0; i <= m_segmentsY; i++) {
			float theta = i * (PI / m_segmentsY);
			Point p1(r*sin(theta)*cos(phi), r*cos(theta), r*sin(theta)*sin(phi));
			Point p2(r*sin(theta)*cos(phi + (2 * PI / m_segmentsX)), r*cos(theta), r*sin(theta)*sin(phi + (2 * PI / m_segmentsX)));
			//Point origin = Point(0.0, 0.0, 0.0);
			//Vector n1 = (p1 - origin);
			//Vector n2 = (p2 - origin);
			//n1.normalize(); n2.normalize();

			if (i == 0 || i == m_segmentsY) {
				face.push_back(point_info(p1, Vector(0.0, 0.0, 0.0)));
				face.push_back(point_info(p2, Vector(0.0, 0.0, 0.0)));
			}
			else {
				face.push_back(point_info(p1, sphereBodyVector(p1)));
				face.push_back(point_info(p2, sphereBodyVector(p2)));
			}
		}
	};

	Vector sphereBodyVector(Point p) {
		Vector normal = Vector(p.at(0), p.at(1), p.at(2));
		normal.normalize();
		normal = normal * 0.1;
		return normal;
	}

	std::vector< std::vector<point_info> > body;
	Matrix m_hmatrix;
};

#endif
