#include "../Public/Vector.h"
#include "../Public/Quaternion.h"
#include <sstream>

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913
#define PI 3.14159265358979323846f
#define TOLERANCE 0.000001f

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

Vec2 Vec2::X()
{
	return Vec2(1, 0);
}

Vec2 Vec2::Y()
{
	return Vec2(1, 0);
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
const Vec2 operator-(const Vec2 & V1)
{
	return Vec2(-V1.x, -V1.y);
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
	for (int i = 0; i < 2; i++) if (std::abs(V1[i] - V2[i]) > TOLERANCE) return false;
	return true;
}
const bool operator!=(const Vec2 & V1, const Vec2 & V2)
{
	for (int i = 0; i < 2; i++) if (std::abs(V1[i] - V2[i]) > TOLERANCE) return true;
	return false;
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

void Vec2::Clean()
{
	x = x <= TOLERANCE ? 0.f : x;
	y = y <= TOLERANCE ? 0.f : y;
}

const Vec2 Normalized(const Vec2& V)
{
	Vec2 normalized = Vec2();
	float len = V.Length();

	normalized.x = V.x / len;
	normalized.y = V.y / len;
	return normalized;
}

const float Dot(const Vec2& V1, const Vec2& V2)
{
	return (V1.x*V2.x + V1.y*V2.y);
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

Vec3::Vec3(const Quat & Q)
{
	x = Q.x;
	y = Q.y;
	z = Q.z;
}

Vec3 Vec3::X()
{
	return Vec3(1, 0, 0);
}

Vec3 Vec3::Y()
{
	return Vec3(0, 1, 0);
}

Vec3 Vec3::Z()
{
	return Vec3(0, 0, 1);
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
const Vec3 operator-(const Vec3 & V1)
{
	return Vec3(-V1.x, -V1.y, -V1.z);
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
	for (int i = 0; i < 3; i++) if (std::abs(V1[i] - V2[i]) > TOLERANCE) return false;
	return true;
}
const bool operator!=(const Vec3 & V1, const Vec3 & V2)
{
	for (int i = 0; i < 3; i++) if (std::abs(V1[i] - V2[i]) > TOLERANCE) return true;
	return false;
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

void Vec3::Clean()
{
	x = x <= TOLERANCE ? 0.f : x;
	y = y <= TOLERANCE ? 0.f : y;
	z = z <= TOLERANCE ? 0.f : z;
}

const Vec3 Normalized(const Vec3& V)
{
	Vec3 normalized = Vec3();
	float len = V.Length();

	normalized.x = V.x / len;
	normalized.y = V.y / len;
	normalized.z = V.z / len;
	return normalized;
}

const float Dot(const Vec3& V1, const Vec3& V2)
{
	return (V1.x*V2.x + V1.y*V2.y + V1.z*V2.z);
}

const Vec3 Cross(const Vec3& V1, const Vec3& V2)
{
	return Vec3(V1.y*V2.z - V1.z*V2.y, V1.z*V2.x - V1.x*V2.z, V1.x*V2.y - V1.y*V2.x);
}

const Vec3 RotateVector(const Vec3& V, const Vec3 & a, const float degrees)
{
	Vec3 NewVec = (std::cos(degrees*PI) * V) + (std::sin(degrees*PI)*(Cross(a, V))) + (a * (Dot(a, V)) * (1 - std::cos(degrees*PI)));
	return NewVec;
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

Vec4::Vec4(const Quat & Q)
{
	x = Q.x;
	y = Q.y;
	z = Q.z;
	w = Q.t;
}

Vec4 Vec4::X()
{
	return Vec4(1, 0, 0, 0);
}

Vec4 Vec4::Y()
{
	return Vec4(0, 1, 0, 0);
}

Vec4 Vec4::Z()
{
	return Vec4(0, 0, 1, 0);
}

Vec4 Vec4::W()
{
	return Vec4(0, 0, 0, 1);
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
const Vec4 operator-(const Vec4 & V1)
{
	return Vec4(-V1.x, -V1.y, -V1.z, -V1.w);
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
	for (int i = 0; i < 4; i++) if (std::abs(V1[i] - V2[i]) > TOLERANCE) return false;
	return true;
}
const bool operator!=(const Vec4 & V1, const Vec4 & V2)
{
	for (int i = 0; i < 4; i++) if (std::abs(V1[i] - V2[i]) > TOLERANCE) return true;
	return false;
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

void Vec4::Clean()
{
	x = x <= TOLERANCE ? 0.f : x;
	y = y <= TOLERANCE ? 0.f : y;
	z = z <= TOLERANCE ? 0.f : z;
	w = w <= TOLERANCE ? 0.f : w;
}

const Vec4 Normalized(const Vec4& V)
{
	Vec4 normalized = Vec4();
	float len = Vec3(V).Length();

	normalized.x = V.x/len;
	normalized.y = V.y/len;
	normalized.z = V.z/len;
	normalized.w = 1.f;
	return normalized;
}

const float Dot(const Vec4& V1, const Vec4& V2)
{
	return (V1.x*V2.x + V1.y*V2.y + V1.z*V2.z + V1.w*V2.w);
}

const Quat FromAngleAxis(const Vec4 & axis, float degrees)
{
	Vec4 normalizedAxis = Normalized(axis);
	float a = degrees * (float)DEGREES_TO_RADIANS;
	float s = std::sin(a / 2.0f);

	Quat q = Quat(std::cos(a / 2.0f), normalizedAxis.x * s, normalizedAxis.y * s, normalizedAxis.z * s);
	q.Clean();

	return Normalized(q);
}
