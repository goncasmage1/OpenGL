#pragma once
#include <iostream>
#include <string.h>
#include <math.h>

class Vec3
{
	public:

		float x, y, z;

		Vec3();
		Vec3(float newXYZ);
		Vec3(float newX, float newY, float newZ);

		friend const Vec3 operator+(const Vec3& V1, const Vec3& V2);
		friend const Vec3& operator+=(Vec3& V1, Vec3& V2);
		friend const Vec3 operator-(const Vec3& V1, const Vec3& V2);
		friend const Vec3& operator-=(Vec3& V1, Vec3& V2);
		friend const Vec3 operator*(const Vec3& V1, const Vec3& V2);
		friend const Vec3& operator*=(Vec3& V1, Vec3& V2);
		friend const Vec3 operator/(const Vec3& V1, const Vec3& V2);
		friend const Vec3& operator/=(Vec3& V1, Vec3& V2);
		friend const Vec3 operator*(const Vec3& V, const float& F);
		friend const Vec3 operator*(const float& F, const Vec3& V);
		friend const Vec3 operator*=(const Vec3& V, const float& F);
		friend const Vec3 operator/(const Vec3& V, const float& F);
		friend const Vec3 operator/(const float& F, const Vec3& V);
		friend const Vec3 operator/=(const Vec3& V, const float& F);
		friend std::ostream& operator<<(std::ostream& os, const Vec3& V);

		float* GetData();
		std::string ToString() const;
		float Length() const;
		void Normalize();
		Vec3 GetNormalized() const;
		float Dot(const Vec3& V);
		Vec3 Cross(const Vec3& V);
};
