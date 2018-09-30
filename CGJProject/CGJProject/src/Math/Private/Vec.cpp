#include "../Public/Vec.h"
#include <sstream>

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
	float *arr = new float[3];
		
	arr[0] = x;
	arr[1] = y;
	arr[2] = z;
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
const Vec3& operator+=(Vec3& V1, Vec3& V2)
{
	V1.x += V2.x;
	V1.y += V2.y;
	V1.z += V2.z;
	return V1;
}
const Vec3 operator-(const Vec3& V1, const Vec3& V2)
{
	Vec3 newVec = Vec3();

	newVec.x = V1.x - V2.x;
	newVec.y = V1.y - V2.y;
	newVec.z = V1.z - V2.z;
	return newVec;
}
const Vec3& operator-=(Vec3& V1, Vec3& V2)
{
	V1.x -= V2.x;
	V1.y -= V2.y;
	V1.z -= V2.z;
	return V1;
}
const Vec3 operator*(const Vec3& V1, const Vec3& V2)
{
	Vec3 newVec = Vec3();

	newVec.x = V1.x * V2.x;
	newVec.y = V1.y * V2.y;
	newVec.z = V1.z * V2.z;
	return newVec;
}
const Vec3& operator*=(Vec3& V1, Vec3& V2)
{
	V1.x *= V2.x;
	V1.y *= V2.y;
	V1.z *= V2.z;
	return V1;
}
const Vec3 operator/(const Vec3& V1, const Vec3& V2)
{
	Vec3 newVec = Vec3();

	newVec.x = V1.x / V2.x;
	newVec.y = V1.y / V2.y;
	newVec.z = V1.z / V2.z;
	return newVec;
}
const Vec3& operator /=(Vec3& V1, Vec3& V2)
{
	V1.x /= V2.x;
	V1.y /= V2.y;
	V1.z /= V2.z;
	return V1;
}
const Vec3 operator*(const Vec3& V, const float& F)
{
	Vec3 newVec = Vec3();
	Vec3 floatVec = Vec3(F);

	newVec.x = V.x * floatVec.x;
	newVec.y = V.y * floatVec.y;
	newVec.z = V.z * floatVec.z;
	return newVec;
}
const Vec3 operator*(const float& F, const Vec3& V)
{
	Vec3 newVec = Vec3();
	Vec3 floatVec = Vec3(F);

	newVec.x = V.x * floatVec.x;
	newVec.y = V.y * floatVec.y;
	newVec.z = V.z * floatVec.z;
	return newVec;
}
const Vec3& operator*=(Vec3& V, float& F)
{
	Vec3 floatVec = Vec3(F);

	V.x *= floatVec.x;
	V.y *= floatVec.y;
	V.z *= floatVec.z;
	return V;
}
std::ostream & operator<<(std::ostream & os, const Vec3 & V)
{
	os << V.ToString();
	return os;
}

std::string Vec3::ToString() const
{
	std::ostringstream oss;
	oss << "(" << x << "," << y << "," << z << ")" << std::endl;
	return oss.str();
}

float Vec3::Length() const
{
	return std::sqrt(x*x + y * y + z * z);
}

void Vec3::Normalize()
{
	float len = Length();
	x /= len;
	y /= len;
	z /= len;
}

Vec3 Vec3::GetNormalized() const
{
	Vec3 normalized = Vec3();
	float len = Length();

	normalized.x = x / len;
	normalized.y = y / len;
	normalized.z = z / len;
	return normalized;
}

float Vec3::Dot(const Vec3 & V)
{
	return (x*V.x + y*V.y + z*V.z);
}

Vec3 Vec3::Cross(const Vec3 & V)
{
	Vec3 crossed = Vec3();

	crossed.x = (y*V.z - z*V.y);
	crossed.y = (z*V.x - x*V.z);
	crossed.z = (x*V.y - y*V.x);

	return crossed;
}
