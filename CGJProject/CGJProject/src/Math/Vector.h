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

	static Vec2 X();
	static Vec2 Y();

	friend const Vec2 operator+(const Vec2& V1, const Vec2& V2);
	friend const Vec2 operator-(const Vec2& V1, const Vec2& V2);
	friend const Vec2 operator-(const Vec2& V1);
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
	void Clean();
	friend const Vec2 Normalized(const Vec2& V);
	friend const float Dot(const Vec2& V1, const Vec2& V2);
	friend const Vec2 Lerp(const Vec2& From, const Vec2& To, float progress);
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
	Vec3(const struct Quat& Q);

	static Vec3 X();
	static Vec3 Y();
	static Vec3 Z();

	friend const Vec3 operator+(const Vec3& V1, const Vec3& V2);
	friend const Vec3 operator-(const Vec3& V1, const Vec3& V2);
	friend const Vec3 operator-(const Vec3& V1);
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
	void Clean();
	void Rotate(const Vec3& a, const float degrees);
	friend const Vec3 Normalized(const Vec3& V);
	friend const float Dot(const Vec3& V1, const Vec3& V2);
	friend const Vec3 Cross(const Vec3& V1, const Vec3& V2);
	friend const Vec3 RotateVector(const Vec3& V, const Vec3& a, const float degrees);
	friend const Vec3 Lerp(const Vec3& From, const Vec3& To, float progress);

};

struct Vec4
{
	float x, y, z, w;

	Vec4();
	Vec4(float newXYZW);
	Vec4(float newX, float newY, float newZ, float newW);
	Vec4(const struct Vec2 V);
	Vec4(const struct Vec3 V);
	Vec4(const struct Vec3 V, float f);
	Vec4(const Vec4& newV4);
	Vec4(const struct Quat& Q);

	static Vec4 X();
	static Vec4 Y();
	static Vec4 Z();

	friend const Vec4 operator+(const Vec4& V1, const Vec4& V2);
	friend const Vec4 operator-(const Vec4& V1, const Vec4& V2);
	friend const Vec4 operator-(const Vec4& V1);
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
	void Clean();
	friend const Vec4 Normalized(const Vec4& V);
	friend const float Dot(const Vec4& V1, const Vec4& V2);
	friend struct Quat FromAngleAxis(const Vec4& axis, float degrees);
	friend const Vec4 Lerp(const Vec4& From, const Vec4& To, float progress);

};
