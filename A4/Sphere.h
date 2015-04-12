#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
	Sphere() {};
	~Sphere() {};

	void draw() {
		for (int i = 0; i < body.size(); i++) {
			drawPart(body.at(i), GL_TRIANGLE_STRIP, false);
		}
	};

	void drawNormal() {
		for (int i = 0; i < body.size(); i++) {
			drawPart(body.at(i), GL_TRIANGLE_STRIP, true);
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

	double Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix) {
		eyePointP = invert(transformMatrix) * eyePointP;
		rayV = invert(transformMatrix) * rayV;

		double a = dot(rayV, rayV);

		double b = 2 * ((eyePointP.at(0) * rayV.at(0))
			+ (eyePointP.at(1) * rayV.at(1))
			+ (eyePointP.at(2) * rayV.at(2)));

		double c = (eyePointP.at(0) * eyePointP.at(0))
			+ (eyePointP.at(1) * eyePointP.at(1))
			+ (eyePointP.at(2) * eyePointP.at(2))
			- (0.5 * 0.5);

		double det = (b * b) - 4 * a * c;

		return (det >= 0) ? ((-1 * b) - sqrt(det)) / (2 * a) : -1;
	}

	Vector findIsectNormal(Point eyePoint, Vector ray, double dist, Matrix transformMatrix) {
		eyePoint = invert(transformMatrix) * eyePoint;
		ray = invert(transformMatrix) * ray;

		Point onBody = eyePoint + ray * dist;

		Vector norm = onBody - Point(0, 0, 0);
		norm.normalize();

		return norm;
	}

private:
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

			face.push_back(point_info(p1, sphereBodyVector(p1), false));
			face.push_back(point_info(p2, sphereBodyVector(p2), i == m_segmentsY ? true : false));
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
