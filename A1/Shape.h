#ifndef SHAPE_H
#define SHAPE_H

#include <GL/glui.h>
#include "Algebra.h"

class Shape {
public:
	/* Edit: Matt C.
	I changed point_info to include a boolean that marks the end of each strip. This way,
	computeX marks each final point in a strip as an end point, and drawX calls a new
	glBegin after every point marked as an end point. Benefit: we don't need to calculate
	the number of strips in the draw functions, we just iterate through the face and
	every time we hit an end point call glEnd() and, if its not the last point, a new glBegin().

	This allow allows us to put drawFace() in Shape.h as a generic function that iterates through the
	points of any side given (now called drawPart() ).
	*/

	struct point_info {
		Point p;
		Vector normal;
		bool isEndPt;

		point_info(const Point &_p, const Vector &_norm) {
			p = _p;
			normal = _norm;
			isEndPt = false;
		};
		point_info(const Point &_p, const Vector &_norm, const bool endpt) {
			p = _p;
			normal = _norm;
			isEndPt = endpt;
		};
	};

	Shape() {};
	~Shape() {};

	void setSegments(int x, int y) {
		if (x != m_segmentsX || y != m_segmentsY) {
			m_segmentsX = x;
			m_segmentsY = y;
			recompute();
		}
	}

	virtual void draw() {};
	virtual void drawNormal() {};
	virtual void recompute() {};

protected:
	void normalizeNormal (float x, float y, float z) {
		normalizeNormal (Vector(x, y, z));
	};

	void normalizeNormal (Vector v) {
		v.normalize();
		glNormal3dv(v.unpack());
	};

	/*
		These are now generic functions for drawing a set of a specific type of primitive.
		Performs far fewer calculations than the old version. There is a noticeable performance
		speed-up, particularly when running Spring with high segments.
	*/

	void drawPart(std::vector<point_info> &face, int primitive, bool drawingNormals) {
		int i = 0;
		if (drawingNormals){
			while (i < face.size()){
				glBegin(GL_LINES);
				bool isEnd = false;
				while (!isEnd){
					drawNormalAtVertex(face.at(i));
					isEnd = face.at(i++).isEndPt;
				}
				glEnd();
			}

		}
		else {
			while (i < face.size()){
				glBegin(primitive);
				bool isEnd = false;
				while (!isEnd){
					drawPoint(face.at(i));
					isEnd = face.at(i++).isEndPt;
				}
				glEnd();
			}
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

	int m_segmentsX, m_segmentsY;
};

#endif