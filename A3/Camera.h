

#ifndef CAMERA_H
#define CAMERA_H

#include "Algebra.h"

class Camera {
public:
Camera();
~Camera();
void Orient(Point& eye, Point& focus, Vector& up);
void Orient(Point& eye, Vector& look, Vector& up);
void Reset();
void SetViewAngle (double viewAngle);
void SetNearPlane (double nearPlane);
void SetFarPlane (double farPlane);
void SetScreenSize (int screenWidth, int screenHeight);

Matrix GetProjectionMatrix();
Matrix GetModelViewMatrix();

void RotateV(double angle);
void RotateU(double angle);
void RotateW(double angle);
void Rotate(Point p, Vector axis, double degree);
void Translate(const Vector &v);

Point GetEyePoint();
Vector GetLookVector();
Vector GetUpVector();
double GetViewAngle();
double GetNearPlane();
double GetFarPlane();
int GetScreenWidth();
int GetScreenHeight();

double GetFilmPlanDepth();
double GetScreenWidthRatio();

private:

//ModelView
Point m_eye;
Vector m_up;
Vector m_look;

//Projection
double m_width, m_height, m_near, m_far;
double m_angle;
Matrix m_rotate;

double m_rotU, m_rotV, m_rotW;
};
#endif

