#pragma once
#include <iostream>
#include <string>

struct Quaternion
{
	float t, x, y, z;

	Quaternion();
	Quaternion(float val);
	Quaternion(float newX, float newY, float newZ);
	Quaternion(float newT, float newX, float newY, float newZ);
	Quaternion(struct Vec3 V);
	Quaternion(struct Vec4 V);
	Quaternion(const Quaternion& Q);

	friend const Quaternion operator+(const Quaternion& Q1, const Quaternion& Q2);
	friend const Quaternion operator-(const Quaternion& Q1, const Quaternion& Q2);
	friend const Quaternion operator-(const Quaternion& Q1);
	friend const Quaternion operator*(const Quaternion& Q1, const Quaternion& Q2);
	Quaternion& operator+=(const Quaternion& Q);
	Quaternion& operator-=(const Quaternion& Q);
	Quaternion& operator*=(const Quaternion& Q);
	friend const Quaternion operator*(const Quaternion& Q, const float& F);
	friend const Quaternion operator*(const float& F, const Quaternion& Q);
	Quaternion& operator*=(const float& F);
	friend const bool operator==(const Quaternion& Q1, const Quaternion& Q2);
	friend const bool operator!=(const Quaternion& Q1, const Quaternion& Q2);

	friend std::ostream& operator<<(std::ostream& os, const Quaternion& Q);
	friend std::istream& operator>>(std::istream& is, Quaternion& Q);

	float operator[](int row) const;

	struct Mat4 GetMatrix();
	std::string ToString() const;
	void Normalize();
	void Conjugate();
	void Inverse();
	void Clean();
	float Length() const;
	float Quadrance() const;

	friend const Quaternion Normalized(const Quaternion& Q);
	friend const Quaternion Conjugated(const Quaternion& Q);
	friend const Quaternion Inversed(const Quaternion& Q);
	friend const Quaternion FromAngleAxis(const struct Vec4& axis, float degrees);
	friend const struct Vec4 ToAngleAxis(const Quaternion& Q, float degrees);

	friend const Quaternion Lerp(const Quaternion& Q1, const Quaternion& Q2, float alpha);
	friend const Quaternion Slerp(const Quaternion& Q1, const Quaternion& Q2, float alpha);

};

