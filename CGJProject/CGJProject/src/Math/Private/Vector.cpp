#include "../Public/Vec.h"
#include <sstream>


/////////
// Vec2
/////////

Vec2::Vec2() :
	x(0), y(0)
{
}
Vec2::Vec2(float newXY) :
	x(newXY), y(newXY)
{
}
Vec2::Vec2(float newX, float newY) :
	x(newX), y(newY)
{
}

Vec2::Vec2(const Vec3 V) :
	x(V.x), y(V.y)
{
}

Vec2::Vec2(const Vec4 V) :
	x(V.x), y(V.y)
{
}

float* Vec2::GetData()
{
	float *arr = new float[2];

	arr[0] = x;
	arr[1] = y;
	return arr;
}

const Vec2 operator+(const Vec2& V1, const Vec2& V2)
{
	Vec2 newVec = Vec2();

	newVec.x = V1.x + V2.x;
	newVec.y = V1.y + V2.y;

	return newVec;
}
const Vec2& operator+=(Vec2& V1, const Vec2& V2)
{
	V1.x += V2.x;
	V1.y += V2.y;
	return V1;
}
const Vec2 operator-(const Vec2& V1, const Vec2& V2)
{
	Vec2 newVec = Vec2();

	newVec.x = V1.x - V2.x;
	newVec.y = V1.y - V2.y;
	return newVec;
}
const Vec2& operator-=(Vec2& V1, const Vec2& V2)
{
	V1.x -= V2.x;
	V1.y -= V2.y;
	return V1;
}
const Vec2 operator*(const Vec2& V1, const Vec2& V2)
{
	Vec2 newVec = Vec2();

	newVec.x = V1.x * V2.x;
	newVec.y = V1.y * V2.y;
	return newVec;
}
const Vec2& operator*=(Vec2& V1, const Vec2& V2)
{
	V1.x *= V2.x;
	V1.y *= V2.y;
	return V1;
}
const Vec2 operator/(const Vec2& V1, const Vec2& V2)
{
	Vec2 newVec = Vec2();

	newVec.x = V1.x / V2.x;
	newVec.y = V1.y / V2.y;
	return newVec;
}
const Vec2& operator /=(Vec2& V1, const Vec2& V2)
{
	V1.x /= V2.x;
	V1.y /= V2.y;
	return V1;
}
const Vec2 operator*(const Vec2& V, const float& F)
{
	Vec2 newVec = Vec2();
	Vec2 floatVec = Vec2(F);

	newVec.x = V.x * floatVec.x;
	newVec.y = V.y * floatVec.y;
	return newVec;
}
const Vec2 operator*(const float& F, const Vec2& V)
{
	Vec2 newVec = Vec2();
	Vec2 floatVec = Vec2(F);

	newVec.x = V.x * floatVec.x;
	newVec.y = V.y * floatVec.y;
	return newVec;
}
const Vec2& operator*=(Vec2& V, float& F)
{
	Vec2 floatVec = Vec2(F);

	V.x *= floatVec.x;
	V.y *= floatVec.y;
	return V;
}
const bool operator==(const Vec2 & V1, const Vec2 & V2)
{
	return (V1.x == V2.x && V1.y == V2.y);
}
const bool operator!=(const Vec2 & V1, const Vec2 & V2)
{
	return (V1.x == V2.x || V1.y == V2.y);
}
std::ostream & operator<<(std::ostream & os, const Vec2 & V)
{
	os << V.ToString();
	return os;
}

std::istream & operator>>(std::istream & is, Vec2 & V)
{
	is >> V.x >> V.y;
	return is;
}

std::string Vec2::ToString() const
{
	std::ostringstream oss;
	oss << "(" << x << "," << y << ")" << std::endl;
	return oss.str();
}

float Vec2::Length() const
{
	return std::sqrt(x*x + y * y);
}

void Vec2::Normalize()
{
	float len = Length();
	x /= len;
	y /= len;
}

Vec2 Vec2::GetNormalized() const
{
	Vec2 normalized = Vec2();
	float len = Length();

	normalized.x = x / len;
	normalized.y = y / len;
	return normalized;
}

float Vec2::Dot(const Vec2 & V)
{
	return (x*V.x + y*V.y);
}

Vec2 Vec2::Cross(const Vec2 & V)
{
	Vec2 crossed = Vec2();

	//TODO: É possível????
	/*crossed.x = (y*V.z - z * V.y);
	crossed.y = (z*V.x - x * V.z);
	crossed.z = (x*V.y - y * V.x);*/

	return crossed;
}

/////////
// Vec3
/////////

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

Vec3::Vec3(const Vec2 V) :
	x(V.x), y(V.y), z(0)
{
}

Vec3::Vec3(const Vec4 V) :
	x(V.x), y(V.y), z(V.z)
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
const Vec3& operator+=(Vec3& V1, const Vec3& V2)
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
const Vec3& operator-=(Vec3& V1, const Vec3& V2)
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
const Vec3& operator*=(Vec3& V1, const Vec3& V2)
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
const Vec3& operator /=(Vec3& V1, const Vec3& V2)
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
const bool operator==(const Vec3 & V1, const Vec3 & V2)
{
	return (V1.x == V2.x && V1.y == V2.y && V1.z == V2.z);
}
const bool operator!=(const Vec3 & V1, const Vec3 & V2)
{
	return (V1.x == V2.x || V1.y == V2.y || V1.z == V2.z);
}
std::ostream & operator<<(std::ostream & os, const Vec3 & V)
{
	os << V.ToString();
	return os;
}

std::istream & operator>>(std::istream & is, Vec3 & V)
{
	is >> V.x >> V.y >> V.z;
	return is;
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


/////////
// Vec3
/////////

Vec4::Vec4() :
	x(0), y(0), z(0), w(0)
{
}
Vec4::Vec4(float newXYZW) :
	x(newXYZW), y(newXYZW), z(newXYZW), w(newXYZW)
{
}
Vec4::Vec4(float newX, float newY, float newZ, float newW) :
	x(newX), y(newY), z(newZ), w(newW)
{
}

Vec4::Vec4(const Vec2 V) :
	x(V.x), y(V.x), z(0), w(0)
{
}

Vec4::Vec4(const Vec3 V) :
	x(V.x), y(V.y), z(V.z), w(0)
{
}

float* Vec4::GetData()
{
	float *arr = new float[4];

	arr[0] = x;
	arr[1] = y;
	arr[2] = z;
	arr[3] = w;
	return arr;
}

const Vec4 operator+(const Vec4& V1, const Vec4& V2)
{
	Vec4 newVec = Vec4();

	newVec.x = V1.x + V2.x;
	newVec.y = V1.y + V2.y;
	newVec.z = V1.z + V2.z;
	newVec.w = V1.w + V2.w;
	return newVec;
}
const Vec4& operator+=(Vec4& V1, const Vec4& V2)
{
	V1.x += V2.x;
	V1.y += V2.y;
	V1.z += V2.z;
	V1.w += V2.w;
	return V1;
}
const Vec4 operator-(const Vec4& V1, const Vec4& V2)
{
	Vec4 newVec = Vec4();

	newVec.x = V1.x - V2.x;
	newVec.y = V1.y - V2.y;
	newVec.z = V1.z - V2.z;
	newVec.w = V1.w - V2.w;
	return newVec;
}
const Vec4& operator-=(Vec4& V1, const Vec4& V2)
{
	V1.x -= V2.x;
	V1.y -= V2.y;
	V1.z -= V2.z;
	V1.w -= V2.w;
	return V1;
}
const Vec4 operator*(const Vec4& V1, const Vec4& V2)
{
	Vec4 newVec = Vec4();

	newVec.x = V1.x * V2.x;
	newVec.y = V1.y * V2.y;
	newVec.z = V1.z * V2.z;
	newVec.w = V1.w * V2.w;
	return newVec;
}
const Vec4& operator*=(Vec4& V1, const Vec4& V2)
{
	V1.x *= V2.x;
	V1.y *= V2.y;
	V1.z *= V2.z;
	V1.w *= V2.w;
	return V1;
}
const Vec4 operator/(const Vec4& V1, const Vec4& V2)
{
	Vec4 newVec = Vec4();

	newVec.x = V1.x / V2.x;
	newVec.y = V1.y / V2.y;
	newVec.z = V1.z / V2.z;
	newVec.w = V1.w / V2.w;
	return newVec;
}
const Vec4& operator /=(Vec4& V1, const Vec4& V2)
{
	V1.x /= V2.x;
	V1.y /= V2.y;
	V1.z /= V2.z;
	V1.w /= V2.w;
	return V1;
}
const Vec4 operator*(const Vec4& V, const float& F)
{
	Vec4 newVec = Vec4();
	Vec4 floatVec = Vec4(F);

	newVec.x = V.x * floatVec.x;
	newVec.y = V.y * floatVec.y;
	newVec.z = V.z * floatVec.z;
	newVec.w = V.w * floatVec.w;
	return newVec;
}
const Vec4 operator*(const float& F, const Vec4& V)
{
	Vec4 newVec = Vec4();
	Vec4 floatVec = Vec4(F);

	newVec.x = V.x * floatVec.x;
	newVec.y = V.y * floatVec.y;
	newVec.z = V.z * floatVec.z;
	newVec.w = V.w * floatVec.w;
	return newVec;
}
const Vec4& operator*=(Vec4& V, float& F)
{
	Vec4 floatVec = Vec4(F);

	V.x *= floatVec.x;
	V.y *= floatVec.y;
	V.z *= floatVec.z;
	V.w *= floatVec.w;
	return V;
}
const bool operator==(const Vec4 & V1, const Vec4 & V2)
{
	return (V1.x == V2.x && V1.y == V2.y && V1.z == V2.z && V1.w == V2.w);
}
const bool operator!=(const Vec4 & V1, const Vec4 & V2)
{
	return (V1.x == V2.x || V1.y == V2.y || V1.z == V2.z || V1.w == V2.w);
}
std::ostream & operator<<(std::ostream & os, const Vec4 & V)
{
	os << V.ToString();
	return os;
}

std::istream & operator>>(std::istream & is, Vec4 & V)
{
	is >> V.x >> V.y >> V.z >> V.w;
	return is;
}

std::string Vec4::ToString() const
{
	std::ostringstream oss;
	oss << "(" << x << "," << y << "," << z << "," << w << ")" << std::endl;
	return oss.str();
}

float Vec4::Length() const
{
	return std::sqrt(x*x + y*y + z*z + w*w);
}

void Vec4::Normalize()
{
	float len = Length();
	x /= len;
	y /= len;
	z /= len;
	w /= len;
}

Vec4 Vec4::GetNormalized() const
{
	Vec4 normalized = Vec4();
	float len = Length();

	normalized.x = x/len;
	normalized.y = y/len;
	normalized.z = z/len;
	normalized.w = w/len;
	return normalized;
}

float Vec4::Dot(const Vec4 & V)
{
	return (x*V.x + y*V.y + z*V.z + w*V.w);
}

Vec4 Vec4::Cross(const Vec4 & V)
{
	Vec4 crossed = Vec4();

	//TODO: Como se faz???
	/*crossed.x = (y*V.z - z * V.y);
	crossed.y = (z*V.x - x * V.z);
	crossed.z = (x*V.y - y * V.x);*/

	return crossed;
}