#include "..\Public\Quaternion.h"
#include "..\Public\Vector.h"
#include "..\Public\Matrix.h"
#include <sstream>

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913
#define TOLERANCE 0.000001f

Quat::Quat()
{
	t = x = y = z = 0.f;
}

Quat::Quat(float val)
{
	t = x = y = z = val;
}

Quat::Quat(float newX, float newY, float newZ)
{
	t = 0.f;
	x = newX;
	y = newY;
	z = newZ;
}

Quat::Quat(float newT, float newX, float newY, float newZ)
{
	t = newT;
	x = newX;
	y = newY;
	z = newZ;
}

Quat::Quat(Vec3 V)
{
	t = 0.f;
	x = V.x;
	y = V.y;
	z = V.z;
}

Quat::Quat(Vec4 V)
{
	t = V.w;
	x = V.x;
	y = V.y;
	z = V.z;
}

Quat::Quat(const Quat & Q)
{
	t = Q.t;
	x = Q.x;
	y = Q.y;
	z = Q.z;
}

const Quat operator+(const Quat & Q1, const Quat & Q2)
{
	return Quat(Q1.t + Q2.t, Q1.x + Q2.x, Q1.y + Q2.y, Q1.z + Q2.z);
}

const Quat operator-(const Quat & Q1, const Quat & Q2)
{
	return Quat(Q1.t - Q2.t, Q1.x - Q2.x, Q1.y - Q2.y, Q1.z - Q2.z);
}

const Quat operator-(const Quat & Q1)
{
	return Quat(-Q1.t, -Q1.x, -Q1.y, -Q1.z);
}

const Quat operator*(const Quat & Q1, const Quat & Q2)
{
	return Quat(Q1.t * Q2.t - Q1.x * Q2.x - Q1.y * Q2.y - Q1.z * Q2.z,
				Q1.t * Q2.x + Q1.x * Q2.t + Q1.y * Q2.z - Q1.z * Q2.y,
				Q1.t * Q2.y + Q1.y * Q2.t + Q1.z * Q2.x - Q1.x * Q2.z,
				Q1.t * Q2.z + Q1.z * Q2.t + Q1.x * Q2.y - Q1.y * Q2.x);
}

Quat & Quat::operator+=(const Quat & Q)
{
	t += Q.t;
	x += Q.x;
	y += Q.y;
	z += Q.z;
	return *this;
}

Quat & Quat::operator-=(const Quat & Q)
{
	t -= Q.t;
	x -= Q.x;
	y -= Q.y;
	z -= Q.z;
	return *this;
}

Quat & Quat::operator*=(const Quat & Q)
{
	t = t * Q.t - x * Q.x - y * Q.y - z * Q.z;
	x = t * Q.x + x * Q.t + y * Q.z - z * Q.y;
	y = t * Q.y + y * Q.t + z * Q.x - x * Q.z;
	z = t * Q.z + z * Q.t + x * Q.y - y * Q.x;
	return *this;
}

const Quat operator*(const Quat & Q, const float & F)
{
	return Quat(Q.t * F, Q.x * F, Q.y * F, Q.z * F);
}

const Quat operator*(const float & F, const Quat & Q)
{
	return Quat(Q.t * F, Q.x * F, Q.y * F, Q.z * F);
}

Quat & Quat::operator*=(const float & F)
{
	t *= F;
	x *= F;
	y *= F;
	z *= F;
	return *this;
}

float Quat::operator[](int row) const
{
	switch (row)
	{
		case 0:
			return t;
		case 1:
			return x;
		case 2:
			return y;
		case 3:
			return z;
		default:
			return 0.0f;
	}
}

std::string Quat::ToString() const
{
	std::ostringstream oss;
	oss << "(" << t << "," << x << "," << y << "," << z << ")" << std::endl;
	return oss.str();
}

const bool operator==(const Quat & Q1, const Quat & Q2)
{
	for (int i = 0; i < 4; i++) if (std::abs(Q1[i] - Q2[i]) > TOLERANCE) return false;
	return true;
}

const bool operator!=(const Quat & Q1, const Quat & Q2)
{
	for (int i = 0; i < 4; i++) if (std::abs(Q1[i] - Q2[i]) > TOLERANCE) return true;
	return false;
}

std::ostream & operator<<(std::ostream & os, const Quat & Q)
{
	os << Q.ToString();
	return os;
}

std::istream & operator>>(std::istream & is, Quat & Q)
{
	is >> Q.t >> Q.x >> Q.y >> Q.z;
	return is;
}

const Quat Normalized(const Quat & Q)
{
	return (Q * (1 / Q.Length()));
}

const Quat Conjugated(const Quat & Q)
{
	return Quat(Q.t, -Q.x, -Q.y, -Q.z);
}

const Quat Inversed(const Quat & Q)
{
	return (Conjugated(Q) * (1.f / Q.Quadrance()));
}

void Quat::Normalize()
{
	Quat norm = Normalized(*this);
	t = norm.t;
	x = norm.x;
	y = norm.y;
	z = norm.z;
}

void Quat::Conjugate()
{
	Quat conj = Conjugated(*this);
	t = conj.t;
	x = conj.x;
	y = conj.y;
	z = conj.z;
}

void Quat::Inverse()
{
	Quat inv = Inversed(*this);
	t = inv.t;
	x = inv.x;
	y = inv.y;
	z = inv.z;
}

void Quat::Clean()
{
	t = std::abs(t) <= TOLERANCE ? 0.f : t;
	x = std::abs(x) <= TOLERANCE ? 0.f : x;
	y = std::abs(y) <= TOLERANCE ? 0.f : y;
	z = std::abs(z) <= TOLERANCE ? 0.f : z;
}

float Quat::Length() const
{
	return std::sqrt(Quadrance());
}

float Quat::Quadrance() const
{
	return t*t + x*x + y*y + z*z;
}

const void ToAngleAxis(const Quat& Q, float& degrees, struct Vec4& axis)
{
	Quat quat = Normalized(Q);
	degrees = 2.0f * std::acos(quat.t) * (float)RADIANS_TO_DEGREES;
	float s = std::sqrt(1.0f - quat.t * quat.t);
	 
	if (s < TOLERANCE)
	{
		axis.x = 1.f;
		axis.y = 0.f;
		axis.z = 0.f;
		axis.w = 1.f;
	}
	else
	{
		axis.x = quat.x / s;
		axis.y = quat.y / s;
		axis.z = quat.z / s;
		axis.w = 1.f;
	}
}

Mat4 Quat::GetMatrix()
{
	Quat qn = Normalized(*this);

	float xx = qn.x * qn.x;
	float xy = qn.x * qn.y;
	float xz = qn.x * qn.z;
	float xt = qn.x * qn.t;
	float yy = qn.y * qn.y;
	float yz = qn.y * qn.z;
	float yt = qn.y * qn.t;
	float zz = qn.z * qn.z;
	float zt = qn.z * qn.t;

	Mat4 matrix = Mat4();

	matrix[0][0] = 1.0f - 2.0f * (yy + zz);
	matrix[1][0] = 2.0f * (xy + zt);
	matrix[2][0] = 2.0f * (xz - yt);
	matrix[3][0] = 0.0f;

	matrix[0][1] = 2.0f * (xy - zt);
	matrix[1][1] = 1.0f - 2.0f * (xx + zz);
	matrix[2][1] = 2.0f * (yz + xt);
	matrix[3][1] = 0.0f;

	matrix[0][2] = 2.0f * (xz + yt);
	matrix[1][2] = 2.0f * (yz - xt);
	matrix[2][2] = 1.0f - 2.0f * (xx + yy);
	matrix[3][2] = 0.0f;

	matrix[0][3] = 0.0f;
	matrix[1][3] = 0.0f;
	matrix[2][3] = 0.0f;
	matrix[3][3] = 1.0f;

	matrix.Clean();

	return matrix;
}

const Quat Lerp(const Quat & Q1, const Quat & Q2, float alpha)
{
	float angle = Q1.t * Q2.t + Q1.x * Q2.x + Q1.y * Q2.y + Q1.z * Q2.z;
	float a1 = 1.f - alpha;
	float a2 = angle > 0 ? alpha : -alpha;

	return Normalized(Quat(Q1*a1) + Quat(Q2*a2));
}

const Quat Slerp(const Quat & Q1, const Quat & Q2, float alpha)
{
	float angle = std::acos(Q1.t * Q2.t + Q1.x * Q2.x + Q1.y * Q2.y + Q1.z * Q2.z);
	float a1 = std::sin((1-alpha)*angle) / std::sin(angle);
	float a2 = std::sin(alpha*angle) / std::sin(angle);

	return Normalized(Quat(Q1*a1) + Quat(Q2*a2));
}
