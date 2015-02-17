#ifndef SPRING_H
#define SPRING_H

#include "Shape.h"

const int SPRING_LOOPS = 4;
const float R_COIL = 0.5;
const float R_TUBE = 0.05;
const float TUBE_SPD = 0.1;

class Spring : public Shape {
public:
	Spring() {};
	~Spring() {};

	void draw() {
		for (int i = 0; i < body.size(); i++) {
			drawPart(body.at(i), GL_TRIANGLE_STRIP, false);
		}
		drawPart(caps[START], GL_TRIANGLE_FAN, false);
		drawPart(caps[END], GL_TRIANGLE_FAN, false);
	};

	void drawNormal() {
		for (int i = 0; i < body.size(); i++) {
			drawPart(body.at(i), GL_TRIANGLE_STRIP ,true);
		}
		drawPart(caps[START], GL_TRIANGLE_FAN, true);
		drawPart(caps[END], GL_TRIANGLE_FAN, true);
	};

	void recompute() {
		std::cout << "redrawing!" << std::endl;
		//clear Faces
		for (int i = 0; i < body.size(); i++) {
			body[i].clear();
		}
		body.clear();
		m_segmentsX_actual = m_segmentsX * SPRING_LOOPS;

		computeBody(body);
		computeCap(0.0, caps[START], true);
		computeCap(2*PI*SPRING_LOOPS, caps[END], false);
	}
private:
	enum CAP_NAME {START, END, NUM_CAPS};

	void computeBody(std::vector< std::vector<point_info> > &body) {
		for (int i = 0; i < m_segmentsX_actual; i++) {
			std::vector< point_info > face;
			computeStrip(i * (2 * SPRING_LOOPS * PI / m_segmentsX_actual), face);
			body.push_back(face);
		}
	};

	void computeStrip(float coil_angle, std::vector<point_info> &face){
		for (int i = 0; i <= m_segmentsY; i++) {
			float tube_angle = i * (2*PI / m_segmentsY);
			Point p1(Xcoord(coil_angle, tube_angle, R_TUBE),
				Ycoord(coil_angle, tube_angle, R_TUBE),
				Zcoord(coil_angle, tube_angle, R_TUBE));
			Point p2(Xcoord(coil_angle + (2 * SPRING_LOOPS * PI / m_segmentsX_actual), tube_angle, R_TUBE),
				Ycoord(coil_angle + (2 * SPRING_LOOPS * PI / m_segmentsX_actual), tube_angle, R_TUBE),
				Zcoord(coil_angle + (2 * SPRING_LOOPS * PI / m_segmentsX_actual), tube_angle, R_TUBE));

			face.push_back(point_info(p1, springBodyVector(p1, coil_angle), false));
			face.push_back(point_info(p2, springBodyVector(p2, coil_angle + (2 * SPRING_LOOPS * PI / m_segmentsX_actual)),
				i == m_segmentsY ? true : false));
		}
	};

	void computeCap(float coil_angle, std::vector<point_info> &cap, bool isStart){
		Point c(Xcoord(coil_angle, 0, 0),
			Ycoord(coil_angle, 0, 0),
			Zcoord(coil_angle, 0, 0));
		Point n1(Xcoord(coil_angle, 0.0, 1.0),
			Ycoord(coil_angle, 0, 1.0),
			Zcoord(coil_angle, 0, 1.0));
		float angle2 = (isStart ? -PI / 2 : PI / 2);
		Point n2(Xcoord(coil_angle, angle2, 1.0),
			Ycoord(coil_angle, angle2, 1.0),
			Zcoord(coil_angle, angle2, 1.0));
		Vector cap_norm = cross(n1 - c, n2 - c);
		cap_norm.normalize();
		cap.push_back(point_info(c,cap_norm));

		for (int i = 0; i <= m_segmentsY; i++) {
			float tube_angle = (isStart ? -1 : 1) * i * (2 * PI / m_segmentsY);
			Point p(Xcoord(coil_angle, tube_angle, R_TUBE),
				Ycoord(coil_angle, tube_angle, R_TUBE),
				Zcoord(coil_angle, tube_angle, R_TUBE));
			cap.push_back(point_info(p, cap_norm, i == m_segmentsY ? true : false));
		}
	}

	float Xcoord(float u, float v, float r){
		return (R_COIL + r*cos(v))*cos(u);
	}

	float Zcoord(float u, float v, float r){
		return (R_COIL + r*cos(v))*sin(u);
	}

	float Ycoord(float u, float v, float r){
		return r*sin(v) + TUBE_SPD*u/PI - TUBE_SPD*SPRING_LOOPS;
	}

	Vector springBodyVector(Point p, float u) {
		Point c(Xcoord(u, 0, 0),
			Ycoord(u, 0, 0),
			Zcoord(u, 0, 0));
		Vector normal = p - c;
		normal.normalize();
		return normal;
	}

	std::vector< std::vector<point_info> > body;
	std::vector<point_info> caps[NUM_CAPS];
	float m_segmentsX_actual;
};

#endif
