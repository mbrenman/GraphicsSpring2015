#ifndef CONE_H
#define CONE_H

#include "Shape.h"

class Cone : public Shape {
public:
	Cone() {};
	~Cone() {};

	void draw() {
		for (int i = 0; i < body.size(); i++) {
			drawPart(body.at(i), GL_TRIANGLE_STRIP, false);
		}
		drawPart(base, GL_TRIANGLE_FAN, false);
	};

	void drawNormal() {
		for (int i = 0; i < body.size(); i++) {
			drawPart(body.at(i), GL_TRIANGLE_STRIP, true);
		}
		drawPart(base, GL_TRIANGLE_FAN, true);
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
		Point topp(0.0, 0.5, 0.0);
		computeBody(startp, nextp, topp, body);
		computeBase(Point(0.0, -0.5, 0.0), startp, nextp, base, Vector(0.0, -1.0, 0.0));
	}

	intersect_info Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix) {
		eyePointP = invert(transformMatrix) * eyePointP;
		rayV = invert(transformMatrix) * rayV;
		intersect_info info;
		double min_t = -1;
		Vector min_vector;
		bool hit_cap = false;

		double a = rayV.at(0) * rayV.at(0) + 
				   rayV.at(2) * rayV.at(2) -
				   (0.25 * rayV.at(1) * rayV.at(1));

		double b = 2 * (eyePointP.at(0) * rayV.at(0))
				 + 2 * (eyePointP.at(2) * rayV.at(2))
				 + 0.25 * rayV.at(1)
				 - 0.5 * (eyePointP.at(1) * rayV.at(1));

		double c = (eyePointP.at(0) * eyePointP.at(0))
			+ (eyePointP.at(2) * eyePointP.at(2))
			- 0.25 * (eyePointP.at(1) * eyePointP.at(1))
			+ 0.25 * (eyePointP.at(1))
			- (1.0/16.0);

		double det = (b * b) - 4 * a * c;

		double t = (det >= 0) ? ((-1 * b) - sqrt(det)) / (2 * a) : -1;
		if (testBounds(eyePointP, rayV, t)) {
			min_t = t;
		}

		//plane (0,-0.5,0)
		if (rayV.at(1) != 0) {
			t = (-0.5 - eyePointP.at(1)) / rayV.at(1);
			if (((t < min_t) || (min_t < 0)) && (t > 0) && testCapBounds(eyePointP, rayV, t)) {
				min_t = t;
				hit_cap = 1;
			}
		}

		if (min_t > 0) {
			if (hit_cap) {
				min_vector = Vector(0, -1, 0);
			}
			else {
				min_vector = coneBodyVector(eyePointP + rayV * min_t);
			}
		}
		
		info.t = min_t;
		info.normal = min_vector;
		return info;
	}

private:
	void computeBody(Point startp, Point nextp, Point topp, std::vector< std::vector<point_info> > &body) {
		Vector rot_vec = nextp - startp;
		Vector tess_vec1, tess_vec2;

		for (int i = 0; i < m_segmentsX; i++) {
			tess_vec1 = (topp - startp) / m_segmentsY;
			tess_vec2 = (topp - nextp) / m_segmentsY;

			std::vector< point_info > face;
			computeStrip(startp, nextp, tess_vec1, tess_vec2, face);
			startp = nextp;
			rot_vec = m_rmatrix * rot_vec;
			nextp = nextp + rot_vec;
			body.push_back(face);
		}
	};

	void computeStrip(Point startp, Point nextp, Vector tess_vec1, Vector tess_vec2, std::vector<point_info> &face){
		for (int i = 0; i <= m_segmentsY; i++) {
			if (i == m_segmentsY) {
				face.push_back(point_info(startp, Vector(0, 0, 0), false));
				face.push_back(point_info(nextp, Vector(0, 0, 0), true));
			}
			else {
				face.push_back(point_info(startp, coneBodyVector(startp)));
				face.push_back(point_info(nextp, coneBodyVector(nextp)));
			}
			startp = startp + tess_vec1;
			nextp = nextp + tess_vec2;
		}
	};

	Vector coneBodyVector(Point p) {
		Vector normal = Vector(p.at(0), 0, p.at(2));
		normal.normalize();
		normal = Vector(normal.at(0), 0.5, normal.at(2));
		normal.normalize();
		return normal;
	}

	void computeBase(Point centerpt, Point edgept1, Point edgept2, std::vector<point_info> &base, Vector normal){
		normal.normalize();
		base.clear();
		base.push_back(point_info(centerpt, normal));
		Vector rot_vec = edgept2 - edgept1;
		for (int i = 0; i <= m_segmentsX; i++){
			base.push_back(point_info(edgept1, normal, i == m_segmentsX ? true : false));
			edgept1 = edgept1 + rot_vec;
			rot_vec = m_rmatrix * rot_vec;
		}
	};

	std::vector< std::vector<point_info> > body;
	std::vector< point_info > base;
	Matrix m_rmatrix;

	bool testBounds(Point eye, Vector ray, double t) {
		double y = (eye + t*ray).at(1);
		return (y >= -0.5) && (y <= 0.5) && (t > 0);
	}

	bool testCapBounds(Point eye, Vector ray, double t) {
		Point p = eye + t*ray;
		return (p.at(0) * p.at(0) + p.at(2) * p.at(2) <= 0.25) && (t > 0);
	}
};

#endif
