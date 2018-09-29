#include "Vec.h"


	Vec3::Vec3(): 
		x(0), y(0), z(0)
	{
	}
	Vec3::Vec3(float newXYZ) :
		x(newXYZ), y(newXYZ), z(newXYZ)
	{
	}
	Vec3::Vec3(float newX, float newY, float newZ) :
		x(newX), y(newY), z(newZ)
	{
	}

	float* Vec3::GetData()
	{
		float arr[3] = { x, y, z };
		return arr;
	}

	const Vec3 operator+(const Vec3& V1, const Vec3& V2)
	{
		Vec3 newVec = Vec3();

		newVec.x = V1.x + V2.x;
		newVec.y = V1.y + V2.y;
		newVec.z = V1.z + V2.z;

		return newVec;
	}
	const Vec3& operator+=(const Vec3& V1, const Vec3& V2)
	{
		this->x += V.x;
		this->y += V.y;
		this->z += V.z;
		return *this;
	}
	const Vec3 operator-(const Vec3& V1, const Vec3& V2)
	{
		Vec3 newVec = Vec3();

		newVec.x = V1.x - V2.x;
		newVec.y = V1.y - V2.y;
		newVec.z = V1.z - V2.z;
		return newVec;
	}
	const Vec3& operator-=(const Vec3& V1, const Vec3& V2)
	{
		this->x -= V.x;
		this->y -= V.y;
		this->z -= V.z;
		return *this;
	}
	const Vec3 operator*(const Vec3& V1, const Vec3& V2)
	{
		Vec3 newVec = Vec3();

		newVec.x = V1.x * V2.x;
		newVec.y = V1.y * V2.y;
		newVec.z = V1.z * V2.z;
		return newVec;
	}
	const Vec3& operator*=(const Vec3& V1, const Vec3& V2)
	{
		this->x *= V.x;
		this->y *= V.y;
		this->z *= V.z;
		return *this;
	}
	const Vec3 operator/(const Vec3& V1, const Vec3& V2)
	{
		Vec3 newVec = Vec3();

		newVec.x = V1.x / V2.x;
		newVec.y = V1.y / V2.y;
		newVec.z = V1.z / V2.z;
		return newVec;
	}
	const Vec3& operator /=(const Vec3& V1, const Vec3& V2)
	{
		this->x /= V.x;
		this->y /= V.y;
		this->z /= V.z;
		return *this;
	}
	const Vec3 operator*(const Vec3& V, const float& F)
	{
		Vec3 newVec = Vec3();
		Vec3 floatVec = Vec3(F);

		newVec.x = V.x + floatVec.x;
		newVec.y = V.y + floatVec.y;
		newVec.z = V.z + floatVec.z;
		return newVec;
	}
	const Vec3 operator*(const float& F, const Vec3& V)
	{
		Vec3 newVec = Vec3();
		Vec3 floatVec = Vec3(F);

		newVec.x = V.x + floatVec.x;
		newVec.y = V.y + floatVec.y;
		newVec.z = V.z + floatVec.z;
		return newVec;
	}
	const Vec3& operator*=(const Vec3& V, const float& F)
	{
		Vec3 floatVec = Vec3(F);

		this->x *= floatVec.x;
		this->y *= floatVec.y;
		this->z *= floatVec.z;
		return *this;
	}
	const Vec3 operator/(const Vec3& V, const float& F)
	{
		Vec3 newVec = Vec3();
		Vec3 floatVec = Vec3(F);

		newVec.x = this->x / floatVec.x;
		newVec.y = this->y / floatVec.y;
		newVec.z = this->z / floatVec.z;
		return newVec;
	}
	const Vec3 operator/(const float& F, const Vec3& V)
	{
		Vec3 newVec = Vec3();
		Vec3 floatVec = Vec3(F);

		newVec.x = this->x / floatVec.x;
		newVec.y = this->y / floatVec.y;
		newVec.z = this->z / floatVec.z;
		return newVec;
	}
	const Vec3& operator/=(const Vec3& V, const float& F)
	{
		Vec3 floatVec = Vec3(F);

		this->x /= floatVec.x;
		this->y /= floatVec.y;
		this->z /= floatVec.z;
		return *this;
	}
