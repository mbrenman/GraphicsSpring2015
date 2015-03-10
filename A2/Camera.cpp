#include "Camera.h"
#include <iostream>

Camera::Camera() {
	//Translation
	m_eye = Point(0.0, 0.0, 0.0);
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
	Matrix m;

	// Matrix scaling = Matrix(2/m_width, 0, 0, 0,
	// 						0, 2/m_height, 0, 0,
	// 						0, 0, 1/m_far, 0,
	// 						0, 0, 0, 1);

	Matrix scaling = Matrix(1/atan((m_angle) / 2) * m_far, 0, 0, 0,
							0, 1/atan((m_angle / GetScreenWidthRatio()) / 2) * m_far, 0, 0,
							0, 0, 1/m_far, 0,
							0, 0, 0, 1);



	// double c = (-1 * m_near) / m_far;
	// Matrix unhinge = Matrix(1, 0, 0, 0,
	// 						0, 1, 0, 0,
	// 						0, 0, (-1 / (c + 1)), ((c / c + 1)),
	// 						0, 0, -1, 0);

	m = scaling;

	return m;
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
	Matrix m;

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

	m = toWorld * translate;



	return m;
}

void Camera::RotateV(double angle) {
}

void Camera::RotateU(double angle) {
}

void Camera::RotateW(double angle) {
}

void Camera::Translate(const Vector &v) {
}


void Camera::Rotate(Point p, Vector axis, double degrees) {
	// double theta, thetaprime;
	// Matrix my, mz, mx, imz, imy;
	// Vector axisprime;
	// theta = atan2(axis.at(2), axis.at(0));
	// my = rotY_mat(theta);

	// axisprime = my * axis;
	// thetaprime = atan2(axisprime.at(1), axisprime.at(0));
	// mz = rotZ_mat(thetaprime);

	// mx = rotX_mat(DEG_TO_RAD(degrees));

	// imz = inv_rotZ_mat(thetaprime);
	// imy = inv_rotZ_mat(theta);

	// m_rotmat = imy * imz * mx * mz * my;
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
	return 0;
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
