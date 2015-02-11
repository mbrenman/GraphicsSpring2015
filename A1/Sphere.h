#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
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

		computeBody(body);
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
		Vector norm = 0.1 * p_info.normal;
		glVertex3f(p.at(0), p.at(1), p.at(2));

		p = p + norm;
		glVertex3f(p.at(0), p.at(1), p.at(2));
	};

	void computeBody(std::vector< std::vector<point_info> > &body) {		
		for (int i = 0; i < m_segmentsX; i++) {
			std::vector< point_info > face;
			computeStrip(0.5, i * (2 * PI / m_segmentsX), face);
			body.push_back(face);
		}
	};

	void computeStrip(float r, float theta, std::vector<point_info> &face){
		for (int i = 0; i <= m_segmentsY; i++) {
			float phi = i * (PI / m_segmentsY);
			Point p1(r*sin(phi)*cos(theta), r*cos(phi), r*sin(phi)*sin(theta));
			Point p2(r*sin(phi)*cos(theta + (2 * PI / m_segmentsX)), r*cos(phi), r*sin(phi)*sin(theta + (2 * PI / m_segmentsX)));

			face.push_back(point_info(p1, sphereBodyVector(p1)));
			face.push_back(point_info(p2, sphereBodyVector(p2)));
		}
	};

	Vector sphereBodyVector(Point p) {
		Vector normal = Vector(p.at(0), p.at(1), p.at(2));
		normal.normalize();
		return normal;
	}

	std::vector< std::vector<point_info> > body;
};

#endif
