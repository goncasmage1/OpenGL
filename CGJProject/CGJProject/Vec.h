#pragma once


class Vec3
{
public:

	float x, y, z;

	Vec3();
	Vec3(float newXYZ);
	Vec3(float newX, float newY, float newZ);

	float* GetData();

	friend const Vec3 operator+(const Vec3& V1, const Vec3& V2);
	friend const Vec3& operator+=(const Vec3& V1, const Vec3& V2);
	friend const Vec3 operator-(const Vec3& V1, const Vec3& V2);
	friend const Vec3& operator-=(const Vec3& V1, const Vec3& V2);
	friend const Vec3 operator*(const Vec3& V1, const Vec3& V2);
	friend const Vec3& operator*=(const Vec3& V1, const Vec3& V2);
	friend const Vec3 operator/(const Vec3& V1, const Vec3& V2);
	friend const Vec3& operator/=(const Vec3& V1, const Vec3& V2);
	friend const Vec3 operator*(const Vec3& V, const float& F);
	friend const Vec3 operator*(const float& F, const Vec3& V);
	friend const Vec3& operator*=(const Vec3& V, const float& F);
	friend const Vec3 operator/(const Vec3& V, const float& F);
	friend const Vec3 operator/(const float& F, const Vec3& V);
	friend const Vec3& operator/=(const Vec3& V, const float& F);
};