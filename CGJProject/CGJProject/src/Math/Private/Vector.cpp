#include "../Public/Vector.h"
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

Vec2::Vec2(const Vec2& newV2)
{
	x = newV2.x;
	y = newV2.y;
}

Vec2::Vec2(const Vec3 V) :
	x(V.x), y(V.y)
{
}

Vec2::Vec2(const Vec4 V) :
	x(V.x), y(V.y)
{
}

float* Vec2::GetData() const
{
	float* arr = new float[2]{x,y};
	return arr;
}

const Vec2 operator+(const Vec2& V1, const Vec2& V2)
{
	Vec2 newVec = Vec2();

	newVec.x = V1.x + V2.x;
	newVec.y = V1.y + V2.y;

	return newVec;
}
Vec2 & Vec2::operator+=(const Vec2 & V)
{
	x += V.x;
	y += V.y;
	return *this;
}
Vec2 & Vec2::operator-=(const Vec2 & V)
{
	x -= V.x;
	y -= V.y;
	return *this;
}
Vec2 & Vec2::operator*=(const Vec2 & V)
{
	x *= V.x;
	y *= V.y;
	return *this;
}
Vec2 & Vec2::operator/=(const Vec2 & V)
{
	x /= V.x;
	y /= V.y;
	return *this;
}
const Vec2 operator-(const Vec2& V1, const Vec2& V2)
{
	Vec2 newVec = Vec2();

	newVec.x = V1.x - V2.x;
	newVec.y = V1.y - V2.y;
	return newVec;
}
const Vec2 operator*(const Vec2& V1, const Vec2& V2)
{
	Vec2 newVec = Vec2();

	newVec.x = V1.x * V2.x;
	newVec.y = V1.y * V2.y;
	return newVec;
}
const Vec2 operator/(const Vec2& V1, const Vec2& V2)
{
	Vec2 newVec = Vec2();

	newVec.x = V1.x / V2.x;
	newVec.y = V1.y / V2.y;
	return newVec;
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
Vec2 & Vec2::operator*=(const float & F)
{
	x *= F;
	y *= F;
	return *this;
}
const bool operator==(const Vec2 & V1, const Vec2 & V2)
{
	return (V1.x == V2.x && V1.y == V2.y);
}
const bool operator!=(const Vec2 & V1, const Vec2 & V2)
{
	return (V1.x != V2.x || V1.y != V2.y);
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

float Vec2::operator[](int row) const
{
	switch (row)
	{
		case 0:
			return x;
		case 1:
			return y;
		default:
			return 0.0f;
	}
}

std::string Vec2::ToString() const
{
	std::ostringstream oss;
	oss << "(" << x << "," << y << ")" << std::endl;
	return oss.str();
}

float Vec2::Length() const
{
	return std::sqrt(x*x + y*y);
}

float Vec2::Quadrance()
{
	return (x*x + y*y);
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

Vec3::Vec3(const Vec3 & newV3)
{
	x = newV3.x;
	y = newV3.y;
	z = newV3.z;
}

Vec3::Vec3(const Vec2 V) :
	x(V.x), y(V.y), z(0)
{
}

Vec3::Vec3(const Vec4 V) :
	x(V.x), y(V.y), z(V.z)
{
}

float* Vec3::GetData() const
{
	float* arr = new float[3] { x, y, z };
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
const Vec3 operator-(const Vec3& V1, const Vec3& V2)
{
	Vec3 newVec = Vec3();

	newVec.x = V1.x - V2.x;
	newVec.y = V1.y - V2.y;
	newVec.z = V1.z - V2.z;
	return newVec;
}
const Vec3 operator*(const Vec3& V1, const Vec3& V2)
{
	Vec3 newVec = Vec3();

	newVec.x = V1.x * V2.x;
	newVec.y = V1.y * V2.y;
	newVec.z = V1.z * V2.z;
	return newVec;
}
const Vec3 operator/(const Vec3& V1, const Vec3& V2)
{
	Vec3 newVec = Vec3();

	newVec.x = V1.x / V2.x;
	newVec.y = V1.y / V2.y;
	newVec.z = V1.z / V2.z;
	return newVec;
}
Vec3& Vec3::operator+=(const Vec3& V)
{
	x += V.x;
	y += V.y;
	z += V.z;
	return *this;
}
Vec3& Vec3::operator-=(const Vec3& V)
{
	x -= V.x;
	y -= V.y;
	z -= V.z;
	return *this;
}
Vec3& Vec3::operator*=(const Vec3& V)
{
	x *= V.x;
	y *= V.y;
	z *= V.z;
	return *this;
}
Vec3& Vec3::operator/=(const Vec3& V)
{
	x /= V.x;
	y /= V.y;
	z /= V.z;
	return *this;
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
Vec3& Vec3::operator*=(const float& F)
{
	x *= F;
	y *= F;
	z *= F;
	return *this;
}
const bool operator==(const Vec3 & V1, const Vec3 & V2)
{
	return (V1.x == V2.x && V1.y == V2.y && V1.z == V2.z);
}
const bool operator!=(const Vec3 & V1, const Vec3 & V2)
{
	return (V1.x != V2.x || V1.y != V2.y || V1.z != V2.z);
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

float Vec3::operator[](int row) const
{
	switch (row)
	{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			return 0.0f;
	}
}

std::string Vec3::ToString() const
{
	std::ostringstream oss;
	oss << "(" << x << "," << y << "," << z << ")" << std::endl;
	return oss.str();
}

float Vec3::Length() const
{
	return std::sqrt(x*x + y*y + z*z);
}

float Vec3::Quadrance()
{
	return (x*x + y*y + z*z);
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
// Vec4
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

Vec4::Vec4(const Vec4 & newV4)
{
	x = newV4.x;
	y = newV4.y;
	z = newV4.z;
	w = newV4.w;
}

float* Vec4::GetData() const
{
	float* arr = new float[4] { x, y, z, w };
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
const Vec4 operator-(const Vec4& V1, const Vec4& V2)
{
	Vec4 newVec = Vec4();

	newVec.x = V1.x - V2.x;
	newVec.y = V1.y - V2.y;
	newVec.z = V1.z - V2.z;
	newVec.w = V1.w - V2.w;
	return newVec;
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
const Vec4 operator/(const Vec4& V1, const Vec4& V2)
{
	Vec4 newVec = Vec4();

	newVec.x = V1.x / V2.x;
	newVec.y = V1.y / V2.y;
	newVec.z = V1.z / V2.z;
	newVec.w = V1.w / V2.w;
	return newVec;
}
Vec4& Vec4::operator+=(const Vec4& V)
{
	x += V.x;
	y += V.y;
	z += V.z;
	w += V.w;
	return *this;
}
Vec4& Vec4::operator-=(const Vec4& V)
{
	x -= V.x;
	y -= V.y;
	z -= V.z;
	w -= V.w;
	return *this;
}
Vec4& Vec4::operator*=(const Vec4& V)
{
	x *= V.x;
	y *= V.y;
	z *= V.z;
	w *= V.w;
	return *this;
}
Vec4& Vec4::operator/=(const Vec4& V)
{
	x /= V.x;
	y /= V.y;
	z /= V.z;
	z /= V.w;
	return *this;
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
Vec4& Vec4::operator*=(const float& F)
{
	x *= F;
	y *= F;
	z *= F;
	w *= F;
	return *this;
}
const bool operator==(const Vec4 & V1, const Vec4 & V2)
{
	return (V1.x == V2.x && V1.y == V2.y && V1.z == V2.z && V1.w == V2.w);
}
const bool operator!=(const Vec4 & V1, const Vec4 & V2)
{
	return (V1.x != V2.x || V1.y != V2.y || V1.z != V2.z || V1.w != V2.w);
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

float Vec4::operator[](int row) const
{
	switch (row)
	{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			return 0.0f;
	}
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

float Vec4::Quadrance()
{
	return (x*x + y*y + z*z + w*w);
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
