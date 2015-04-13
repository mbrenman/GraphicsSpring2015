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

	double Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix) {
		eyePointP = invert(transformMatrix) * eyePointP;
		rayV = invert(transformMatrix) * rayV;
		double min_t = -1;

		double a = rayV.at(0) * rayV.at(0) + rayV.at(2) * rayV.at(2);
		double b = 2 * (eyePointP.at(0) * rayV.at(0) + eyePointP.at(2) * rayV.at(2));
		double c = eyePointP.at(0) * eyePointP.at(0) + eyePointP.at(2) * eyePointP.at(2) - 0.25;

		double det = (b * b) - 4 * a * c;

		double t = (det >= 0) ? ((-1 * b) - sqrt(det)) / (2 * a) : -1;
		min_t = testBounds(eyePointP, rayV, t) ? t : min_t;

		//plane (0,0.5,0)
		if (rayV.at(1) != 0) {
			t = (0.5 - eyePointP.at(1)) / rayV.at(1);
			min_t = ((t < min_t) || (min_t < 0)) && (t > 0) && testCapBounds(eyePointP, rayV, t) ? t : min_t;
		}

		//plane (0,-0.5,0)
		if (rayV.at(1) != 0) {
			t = (-0.5 - eyePointP.at(1)) / rayV.at(1);
			min_t = ((t < min_t) || (min_t < 0)) && (t > 0) && testCapBounds(eyePointP, rayV, t) ? t : min_t;
		}
		
		return min_t;
	}

	Vector findIsectNormal(Point eyePoint, Vector ray, double dist, Matrix transformMatrix) {
		Point onBody = eyePoint + ray * dist;

		//Need to test if on cap

		return Vector(onBody.at(0), 0, onBody.at(2));
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

	bool testBounds(Point eye, Vector ray, double t) {
		double y = (eye + t*ray).at(1);
		return (y >= -0.5) && (y <= 0.5) && (t > 0);
	}

	bool testCapBounds(Point eye, Vector ray, double t) {		
		Point p = eye + t*ray;
		return (p.at(0) * p.at(0) + p.at(2) * p.at(2) <= 0.25) && (t > 0);
	}

	std::vector< std::vector<point_info> > body;
	std::vector< point_info > caps[NUM_CAPS];
	Matrix m_rmatrix;
};
#endif
