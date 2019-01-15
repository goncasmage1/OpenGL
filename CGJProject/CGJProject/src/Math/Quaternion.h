#pragma once
#include <iostream>
#include <string>
#include "foundation/PxQuat.h"

struct Quat
{
	float t, x, y, z;

	Quat();
	Quat(float val);
	Quat(float newX, float newY, float newZ);
	Quat(float newT, float newX, float newY, float newZ);
	Quat(struct Vec3 V);
	Quat(float t, struct Vec3 V);
	Quat(struct Vec4 V);
	Quat(const Quat& Q);

	friend const Quat operator+(const Quat& Q1, const Quat& Q2);
	friend const Quat operator-(const Quat& Q1, const Quat& Q2);
	friend const Quat operator-(const Quat& Q1);
	friend const Quat operator*(const Quat& Q1, const Quat& Q2);
	Quat& operator+=(const Quat& Q);
	Quat& operator-=(const Quat& Q);
	Quat& operator*=(const Quat& Q);
	friend const Quat operator*(const Quat& Q, const float& F);
	friend const Quat operator*(const float& F, const Quat& Q);
	Quat& operator*=(const float& F);
	friend const bool operator==(const Quat& Q1, const Quat& Q2);
	friend const bool operator!=(const Quat& Q1, const Quat& Q2);

	friend std::ostream& operator<<(std::ostream& os, const Quat& Q);
	friend std::istream& operator>>(std::istream& is, Quat& Q);

	float operator[](int row) const;

	struct Mat4 GetMatrix();
	std::string ToString() const;
	void Normalize();
	void Conjugate();
	void Inverse();
	void Clean();
	float Length() const;
	float Quadrance() const;
		
	friend const Quat Normalized(const Quat& Q);
	friend const Quat Conjugated(const Quat& Q);
	friend const Quat Inversed(const Quat& Q);
	friend const void ToAngleAxis(const Quat& Q, float& degrees, struct Vec4& axis);
	Quat FromAngleAxis(float theta, Vec4 axis);

	friend const Quat Lerp(const Quat& Q1, const Quat& Q2, float alpha);
	friend const Quat Slerp(const Quat& Q1, const Quat& Q2, float alpha);
	friend const physx::PxQuat ToPhysxQuat(const Quat& Q);

};

