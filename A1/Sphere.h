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
		Point topp(0.0, 0.5, 0.0);
		Vector rot_vec = nextp - startp;
		Vector tess_vec1, tess_vec2;
		
		for (int i = 0; i < m_segmentsX; i++) {
			//Point upp2(0.5*cos(2 * PI / m_segmentsX)*cos(0.5*PI / m_segmentsY), 0.5*sin(2 * PI / m_segmentsX)*sin(0.5 * PI / m_segmentsY), 0.0);
			//Point upp(0.5*cos(0.5*PI / m_segmentsY), 0.5*sin(0.5 * PI / m_segmentsY), 0.0);

			//Vector upVec(0.0, 1.0, 0.0);
			//Vector startInVec = Point(0.0, 0.0, 0.0) - startp;
			//Vector nextInVec = Point(0.0, 0.0, 0.0) - nextp;

			//tess_vec1 = (startp.at(2) / 0.5) * startInVec - upVec; //startp
			//tess_vec2 = (nextp.at(2) / 0.5) * startInVec - upVec;


			std::vector< point_info > face;
			computeStrip(startp, nextp, tess_vec1, tess_vec2, face);
			startp = nextp;
			rot_vec = m_hmatrix * rot_vec;
			nextp = nextp + rot_vec;
			body.push_back(face);
		}
	};

	void computeStrip(Point startp, Point nextp, Vector tess_vec1, Vector tess_vec2, std::vector<point_info> &face){
		
		for (int i = 0; i <= m_segmentsY; i++) {
			if (i == m_segmentsY) {
				face.push_back(point_info(startp, Vector(0, 0, 0)));
				face.push_back(point_info(nextp, Vector(0, 0, 0)));
			}
			else {
				face.push_back(point_info(startp, sphereBodyVector(startp)));
				face.push_back(point_info(nextp, sphereBodyVector(nextp)));
			}
			startp = startp + tess_vec1;
			nextp = nextp + tess_vec2;
		}
	};

	Vector sphereBodyVector(Point p) {
		Vector normal = Vector(p.at(0), 0, p.at(2));
		normal.normalize();
		normal = Vector(normal.at(0), 0.5, normal.at(2));
		normal.normalize();
		normal = normal * 0.1;
		return normal;
	}

	std::vector< std::vector<point_info> > body;
	Matrix m_hmatrix;
};

#endif
