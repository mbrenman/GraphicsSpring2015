#ifndef SHAPE_H
#define SHAPE_H

#include <GL/glui.h>
#include "Algebra.h"

class Shape {
public:
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

	int m_segmentsX, m_segmentsY;
};

#endif