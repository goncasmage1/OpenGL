#pragma once
#include <iostream>
#include <string.h>
#include <math.h>

class Vec2
{
public:

	float x, y;

	Vec2();
	Vec2(float newXY);
	Vec2(float newX, float newY);
	Vec2(const class Vec3 V);
	Vec2(const class Vec4 V);

	friend const Vec2 operator+(const Vec2& V1, const Vec2& V2);
	friend const Vec2& operator+=(Vec2& V1, const Vec2& V2);
	friend const Vec2 operator-(const Vec2& V1, const Vec2& V2);
	friend const Vec2& operator-=(Vec2& V1, const Vec2& V2);
	friend const Vec2 operator*(const Vec2& V1, const Vec2& V2);
	friend const Vec2& operator*=(Vec2& V1, const Vec2& V2);
	friend const Vec2 operator/(const Vec2& V1, const Vec2& V2);
	friend const Vec2& operator/=(Vec2& V1, const Vec2& V2);
	friend const Vec2 operator*(const Vec2& V, const float& F);
	friend const Vec2 operator*(const float& F, const Vec2& V);
	friend const Vec2 operator*=(const Vec2& V, const float& F);
	friend const bool operator==(const Vec2& V1, const Vec2& V2);
	friend const bool operator!=(const Vec2& V1, const Vec2& V2);
	friend std::ostream& operator<<(std::ostream& os, const Vec2& V);
	friend std::istream& operator>>(std::istream& is, Vec2& V);

	float* GetData();
	std::string ToString() const;
	float Length() const;
	void Normalize();
	Vec2 GetNormalized() const;
	float Dot(const Vec2& V);
	Vec2 Cross(const Vec2& V);
};

class Vec3
{
public:

	float x, y, z;

	Vec3();
	Vec3(float newXYZ);
	Vec3(float newX, float newY, float newZ);
	Vec3(const class Vec2 V);
	Vec3(const class Vec4 V);

	friend const Vec3 operator+(const Vec3& V1, const Vec3& V2);
	friend const Vec3& operator+=(Vec3& V1, const Vec3& V2);
	friend const Vec3 operator-(const Vec3& V1, const Vec3& V2);
	friend const Vec3& operator-=(Vec3& V1, const Vec3& V2);
	friend const Vec3 operator*(const Vec3& V1, const Vec3& V2);
	friend const Vec3& operator*=(Vec3& V1, const Vec3& V2);
	friend const Vec3 operator/(const Vec3& V1, const Vec3& V2);
	friend const Vec3& operator/=(Vec3& V1, const Vec3& V2);
	friend const Vec3 operator*(const Vec3& V, const float& F);
	friend const Vec3 operator*(const float& F, const Vec3& V);
	friend const Vec3 operator*=(const Vec3& V, const float& F);
	friend const bool operator==(const Vec3& V1, const Vec3& V2);
	friend const bool operator!=(const Vec3& V1, const Vec3& V2);
	friend std::ostream& operator<<(std::ostream& os, const Vec3& V);
	friend std::istream& operator>>(std::istream& is, Vec3& V);

	float* GetData();
	std::string ToString() const;
	float Length() const;
	void Normalize();
	Vec3 GetNormalized() const;
	float Dot(const Vec3& V);
	Vec3 Cross(const Vec3& V);
};

class Vec4
{
public:

	float x, y, z, w;

	Vec4();
	Vec4(float newXYZW);
	Vec4(float newX, float newY, float newZ, float newW);
	Vec4(const class Vec2 V);
	Vec4(const class Vec3 V);

	friend const Vec4 operator+(const Vec4& V1, const Vec4& V2);
	friend const Vec4& operator+=(Vec4& V1, const Vec4& V2);
	friend const Vec4 operator-(const Vec4& V1, const Vec4& V2);
	friend const Vec4& operator-=(Vec4& V1, const Vec4& V2);
	friend const Vec4 operator*(const Vec4& V1, const Vec4& V2);
	friend const Vec4& operator*=(Vec4& V1, const Vec4& V2);
	friend const Vec4 operator/(const Vec4& V1, const Vec4& V2);
	friend const Vec4& operator/=(Vec4& V1, const Vec4& V2);
	friend const Vec4 operator*(const Vec4& V, const float& F);
	friend const Vec4 operator*(const float& F, const Vec4& V);
	friend const Vec4 operator*=(const Vec4& V, const float& F);
	friend const bool operator==(const Vec4& V1, const Vec4& V2);
	friend const bool operator!=(const Vec4& V1, const Vec4& V2);
	friend std::ostream& operator<<(std::ostream& os, const Vec4& V);
	friend std::istream& operator>>(std::istream& is, Vec4& V);

	float* GetData();
	std::string ToString() const;
	float Length() const;
	void Normalize();
	Vec4 GetNormalized() const;
	float Dot(const Vec4& V);
	Vec4 Cross(const Vec4& V);
};
