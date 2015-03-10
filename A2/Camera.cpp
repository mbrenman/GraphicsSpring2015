#include "Camera.h"
#include <iostream>

Camera::Camera() {
	//Translation
	Orient(Point(0, 0, 1), Vector(0, 0, -1), Vector(0, 1, 0));
	SetViewAngle(0.0);
	SetFarPlane(1.0);
	SetNearPlane(0.001);
	SetScreenSize(1, 1);
	SetViewAngle(0.0);
	m_rotate = Matrix(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

Camera::~Camera() {
}

void Camera::Orient(Point& eye, Point& focus, Vector& up) {
	m_eye = eye;
	m_look = focus - eye;
	m_up = up;
}


void Camera::Orient(Point& eye, Vector& look, Vector& up) {
	m_eye = eye;
	m_look = look;
	m_up = up;
}

Matrix Camera::GetProjectionMatrix() {
	Matrix scale, unhinge;
	double width_angle, height_angle;
	double scale_h, scale_w;
	double c = -m_near / m_far;

	height_angle = DEG_TO_RAD(m_angle);
	width_angle = height_angle / GetScreenWidthRatio();

	scale_h = 1 / (tan(height_angle / 2) * m_far);
	scale_w = 1 / (tan(width_angle / 2) * m_far);

	scale = Matrix(
		scale_w, 0, 0, 0,
		0, scale_h, 0, 0,
		0, 0, 1 / m_far, 0,
		0, 0, 0, 1);

	unhinge = Matrix(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1/(c+1), c/(c + 1),
		0, 0, -1, 0);

	return unhinge * scale;
}


void Camera::SetViewAngle (double viewAngle) {
	m_angle = viewAngle;
}

void Camera::SetNearPlane (double nearPlane) {
	m_near = nearPlane;
}

void Camera::SetFarPlane (double farPlane) {
	m_far = farPlane;
}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {
	m_width = screenWidth;
	m_height = screenHeight;
}

Matrix Camera::GetModelViewMatrix() {
	Matrix translate = Matrix(1, 0, 0, -m_eye.at(0),
			   				  0, 1, 0, -m_eye.at(1), 
			   				  0, 0, 1, -m_eye.at(2),
			   				  0, 0, 0, 1);

	Vector w = m_look;
	w.negate();
	w.normalize();

	Vector u = cross(m_up, w);
	u.normalize();

	Vector v = cross(w, u);

	Matrix toWorld = Matrix(u.at(0), u.at(1), u.at(2), 0,
							v.at(0), v.at(1), v.at(2), 0,
							w.at(0), w.at(1), w.at(2), 0,
							0, 0, 0, 1);

	return m_rotate * toWorld * translate;
}

void Camera::RotateV(double angle) {
	Vector w = m_look;
	w.negate();
	w.normalize();

	Vector u = cross(m_up, w);
	u.normalize();

	Vector v = cross(w, u);

	Rotate(m_eye, v, -angle);
}

void Camera::RotateU(double -angle) {
	Vector w = m_look;
	w.negate();
	w.normalize();

	Vector u = cross(m_up, w);
	u.normalize();

	Rotate(m_eye, u, -angle);
}

void Camera::RotateW(double angle) {
	Vector w = m_look;
	w.negate();
	w.normalize();

	Rotate(m_eye, w, angle);
}

void Camera::Translate(const Vector &v) {
}

void Camera::Rotate(Point p, Vector axis, double degrees) {
	double theta, thetaprime;
	Matrix my, mz, mx, imz, imy, mall;
	Point plook, plookprime, pup, pupprime;
	Vector axisprime;
	theta = atan2(axis.at(2), axis.at(0));
	my = rotY_mat(theta);

	axisprime = my * axis;
	thetaprime = atan2(axisprime.at(1), axisprime.at(0));
	mz = rotZ_mat(thetaprime);

	mx = rotX_mat(DEG_TO_RAD(degrees));

	imz = inv_rotZ_mat(thetaprime);
	imy = inv_rotY_mat(theta);

	mall = imy * imz * mx * mz * my;
	m_rotate = m_rotate * mall;
}


Point Camera::GetEyePoint() {
	// Point p;
	return m_eye;
}

Vector Camera::GetLookVector() {
	return m_look;
}

Vector Camera::GetUpVector() {
	return m_up;
}

double Camera::GetViewAngle() {
	return m_angle;
}

double Camera::GetNearPlane() {
	return m_near;
}

double Camera::GetFarPlane() {
	return m_far;
}

int Camera::GetScreenWidth() {
	return m_width;
}

int Camera::GetScreenHeight() {
	return m_height;
}

double Camera::GetFilmPlanDepth() {
	return 0;
}

double Camera::GetScreenWidthRatio() {
	return m_width / m_height;
}
