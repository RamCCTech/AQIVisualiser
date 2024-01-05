#include "pch.h"
#include "Point3D.h"

Point3D::Point3D(double inX, double inY)
    :mX(0),
    mY(0)
{
    mX = inX;
    mY = inY;
}

Point3D::~Point3D()
{
}

double Point3D::x()
{
    return mX;
}

double Point3D::y()
{
    return mY;
}

void Point3D::setX(double x) {
    mX = x;
}

void Point3D::setY(double y) {
    mY = y;
}
