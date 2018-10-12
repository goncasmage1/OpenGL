#pragma once
#include <iostream>
#include <string.h>
#include <math.h>

struct Vec2
{
	float x, y;

	Vec2();
	Vec2(float newXY);
	Vec2(float newX, float newY);
	Vec2(const Vec2& newV2);
	Vec2(const struct Vec3 V);
	Vec2(const struct Vec4 V);

	friend const Vec2 operator+(const Vec2& V1, const Vec2& V2);
	friend const Vec2 operator-(const Vec2& V1, const Vec2& V2);
	friend const Vec2 operator*(const Vec2& V1, const Vec2& V2);
	friend const Vec2 operator/(const Vec2& V1, const Vec2& V2);
	Vec2& operator+=(const Vec2& V);
	Vec2& operator-=(const Vec2& V);
	Vec2& operator*=(const Vec2& V);
	Vec2& operator/=(const Vec2& V);
	friend const Vec2 operator*(const Vec2& V, const float& F);
	friend const Vec2 operator*(const float& F, const Vec2& V);
	Vec2& operator*=(const float& F);
	friend const bool operator==(const Vec2& V1, const Vec2& V2);
	friend const bool operator!=(const Vec2& V1, const Vec2& V2);

	friend std::ostream& operator<<(std::ostream& os, const Vec2& V);
	friend std::istream& operator>>(std::istream& is, Vec2& V);

	float operator[](int row) const;

	float* GetData() const;
	std::string ToString() const;
	float Length() const;
	float Quadrance();
	void Normalize();
	friend const Vec2 Normalized(const Vec2& V);
	friend const float Dot(const Vec2& V1, const Vec2& V2);
};

struct Vec3
{
	float x, y, z;

	Vec3();
	Vec3(float newXYZ);
	Vec3(float newX, float newY, float newZ);
	Vec3(const struct Vec2 V);
	Vec3(const Vec3& newV3);
	Vec3(const struct Vec4 V);

	friend const Vec3 operator+(const Vec3& V1, const Vec3& V2);
	friend const Vec3 operator-(const Vec3& V1, const Vec3& V2);
	friend const Vec3 operator*(const Vec3& V1, const Vec3& V2);
	friend const Vec3 operator/(const Vec3& V1, const Vec3& V2);
	Vec3& operator+=(const Vec3& V);
	Vec3& operator-=(const Vec3& V);
	Vec3& operator*=(const Vec3& V);
	Vec3& operator/=(const Vec3& V);
	friend const Vec3 operator*(const Vec3& V, const float& F);
	friend const Vec3 operator*(const float& F, const Vec3& V);
	Vec3& operator*=(const float& F);
	friend const bool operator==(const Vec3& V1, const Vec3& V2);
	friend const bool operator!=(const Vec3& V1, const Vec3& V2);

	friend std::ostream& operator<<(std::ostream& os, const Vec3& V);
	friend std::istream& operator>>(std::istream& is, Vec3& V);

	float operator[](int row) const;

	float* GetData() const;
	std::string ToString() const;
	float Length() const;
	float Quadrance();
	void Normalize();
	friend const Vec3 Normalized(const Vec3& V);
	friend const float Dot(const Vec3& V1, const Vec3& V2);
	friend const Vec3 Cross(const Vec3& V1, const Vec3& V2);
	friend const Vec3 RotV(const Vec3& V, const Vec3& a, const float degrees);

	 
};

struct Vec4
{
	float x, y, z, w;

	Vec4();
	Vec4(float newXYZW);
	Vec4(float newX, float newY, float newZ, float newW);
	Vec4(const struct Vec2 V);
	Vec4(const struct Vec3 V);
	Vec4(const Vec4& newV4);

	friend const Vec4 operator+(const Vec4& V1, const Vec4& V2);
	friend const Vec4 operator-(const Vec4& V1, const Vec4& V2);
	friend const Vec4 operator*(const Vec4& V1, const Vec4& V2);
	friend const Vec4 operator/(const Vec4& V1, const Vec4& V2);
	Vec4& operator+=(const Vec4& V);
	Vec4& operator-=(const Vec4& V);
	Vec4& operator*=(const Vec4& V);
	Vec4& operator/=(const Vec4& V);
	friend const Vec4 operator*(const Vec4& V, const float& F);
	friend const Vec4 operator*(const float& F, const Vec4& V);
	Vec4& operator*=(const float& F);
	friend const bool operator==(const Vec4& V1, const Vec4& V2);
	friend const bool operator!=(const Vec4& V1, const Vec4& V2);

	friend std::ostream& operator<<(std::ostream& os, const Vec4& V);
	friend std::istream& operator>>(std::istream& is, Vec4& V);

	float operator[](int row) const;

	float* GetData() const;
	std::string ToString() const;
	float Length() const;
	float Quadrance();
	void Normalize();
	friend const Vec4 Normalized(const Vec4& V);
	friend const float Dot(const Vec4& V1, const Vec4& V2);

};
