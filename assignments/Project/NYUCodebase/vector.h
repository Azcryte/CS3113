#pragma once
#include <math.h>

class Vector {
public:
	Vector();
	Vector(float a, float b, float c = 0.0f);

	float length();
	Vector normalize();

	Vector operator + (const Vector& rhs);
	Vector operator * (float rhs);
	Vector operator - (const Vector& rhs);

	float x;
	float y;
	float z;
};

Vector::Vector() : x(1.0f), y(1.0f), z(1.0f) { normalize(); }

Vector::Vector(float a, float b, float c) : x(a), y(b), z(c) {}

float Vector::length() { 
	return sqrt(x * x + y * y + z * z); 
}

Vector Vector::normalize() {
	float l = length();
	x /= l;
	y /= l;
	z /= l;

	return *this;
}

Vector Vector::operator + (const Vector& rhs) {
	Vector result;

	result.x = x + rhs.x;
	result.y = y + rhs.y;
	result.z = z + rhs.z;

	return result;
}

Vector Vector::operator - (const Vector& rhs) {
	Vector result;

	result.x = x - rhs.x;
	result.y = y - rhs.y;
	result.z = z - rhs.z;

	return result;
}

Vector Vector::operator * (float rhs) {
	Vector result;

	result.x = x * rhs;
	result.y = y * rhs;
	result.z = z * rhs;

	return result;
}