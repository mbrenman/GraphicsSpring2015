#include "Camera.h"
#include <iostream>

Camera::Camera() {
	//Translation
	m_eye = Point(0.0, 0.0, 0.0);

	//Rotation
	m_rotV = 0;
	m_rotU = 0;
	m_rotW = 0;
}

Camera::~Camera() {
}

void Camera::Orient(Point& eye, Point& focus, Vector& up) {
	m_eye = eye;
}


void Camera::Orient(Point& eye, Vector& look, Vector& up) {
	m_eye = eye;
}

Matrix Camera::GetProjectionMatrix() {
	Matrix m;
	return m;
}


void Camera::SetViewAngle (double viewAngle) {
}

void Camera::SetNearPlane (double nearPlane) {
}

void Camera::SetFarPlane (double farPlane) {
}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {
}

Matrix Camera::GetModelViewMatrix() {
	Matrix m;

	Matrix translate = Matrix(1, 0, 0, -m_eye.at(0),
			   				  0, 1, 0, -m_eye.at(1), 
			   				  0, 0, 1, -m_eye.at(2),
			   				  0, 0, 0, 1);

	m = translate;

	return m;
}

void Camera::RotateV(double angle) {
	m_rotV = angle;
}

void Camera::RotateU(double angle) {
	m_rotU = angle;
}

void Camera::RotateW(double angle) {
	m_rotW = angle;
}

void Camera::Translate(const Vector &v) {
}


void Camera::Rotate(Point p, Vector axis, double degrees) {
}


Point Camera::GetEyePoint() {
	// Point p;
	return m_eye;
}

Vector Camera::GetLookVector() {
	Vector v;
	return v;
}

Vector Camera::GetUpVector() {
	Vector v;
	return v;
}

double Camera::GetViewAngle() {
	return 0;
}

double Camera::GetNearPlane() {
	return 0;
}

double Camera::GetFarPlane() {
	return 0;
}

int Camera::GetScreenWidth() {
	return 0;
}

int Camera::GetScreenHeight() {
	return 0;
}

double Camera::GetFilmPlanDepth() {
	return 0;
}

double Camera::GetScreenWidthRatio() {
	return 0;
}
