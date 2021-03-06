#include "Matrix.h"
#include "Vector.h"
#include <sstream>
#include <assert.h>

#define PI 3.14159265358979323846f
#define TOLERANCE 0.000001f

/////////
// Mat2
/////////

Mat2::Mat2()
{

}

Mat2::Mat2(std::array<std::array<float, 2>, 2> newValues)
{
	values = newValues;
}

Mat2::Mat2(const Mat2 & newMat)
{
	values = newMat.values;
}

Mat2::Mat2(const Mat3 & newMat)
{
	int i, j;
	for (i = 0; i < 2; i++) for (j = 0; j < 2; j++) values[i][j] = newMat[i][j];
}

Mat2::Mat2(const Mat4 & newMat)
{
	int i, j;
	for (i = 0; i < 2; i++) for (j = 0; j < 2; j++) values[i][j] = newMat[i][j];
}

std::array<float, 2>& Mat2::operator[](int row)
{
	return values[row];
}

std::array<float, 2> Mat2::operator[](int row) const
{
	return values[row];
}

const Mat2 operator+(const Mat2 & M1, const Mat2 & M2)
{
	return Mat2(
		{
		{
			{M1[0][0] + M2[0][0], M1[0][1] + M2[0][1]},
			{M1[1][0] + M2[1][0], M1[1][1] + M2[1][1]}
		}
		}
	);
}
const Mat2 operator-(const Mat2 & M1, const Mat2 & M2)
{
	return Mat2(
		{
		{
			{M1[0][0] - M2[0][0], M1[0][1] - M2[0][1]},
			{M1[1][0] - M2[1][0], M1[1][1] - M2[1][1]}
		}
		}
	);
}
const Mat2 operator*(const Mat2 & M1, const Mat2 & M2)
{
	return Mat2(
		{
		{
			{
			M1[0][0] * M2[0][0] + M1[0][1] * M2[1][0],
			M1[0][0] * M2[0][1] + M1[0][1] * M2[1][1]
			},
			{
			M1[1][0] * M2[0][0] + M1[1][1] * M2[1][0],
			M1[1][0] * M2[0][1] + M1[1][1] * M2[1][1]
			}
		}
		}
	);
}
Mat2& Mat2::operator+=(const Mat2& M)
{
	int i, j;
	for (i = 0; i < 2; i++) for (j = 0; j < 2; j++) values[i][j] += M[i][j];
	return *this;
}
Mat2& Mat2::operator-=(const Mat2& M)
{
	int i, j;
	for (i = 0; i < 2; i++) for (j = 0; j < 2; j++) values[i][j] -= M[i][j];
	return *this;
}
Mat2& Mat2::operator*=(const Mat2& M)
{
	values[0][0] = values[0][0] * M[0][0] + values[0][1] * M[1][0];
	values[0][1] = values[0][0] * M[0][1] + values[0][1] * M[1][1];
	values[1][0] = values[1][0] * M[0][0] + values[1][1] * M[1][0];
	values[1][1] = values[1][0] * M[0][1] + values[1][1] * M[1][1];
	return *this;
}
const Mat2 operator*(const Mat2 & M1, const float & F)
{
	return Mat2(
		{
		{
			{M1[0][0] * F, M1[0][1] * F},
			{M1[1][0] * F, M1[1][1] * F}
		}
		}
	);
}
const Mat2 operator*(const float & F, const Mat2 & M1)
{
	Mat2 newMat = Mat2();
	int i, j;
	for (i = 0; i < 2; i++) for (j = 0; j < 2; j++) newMat[i][j] = M1[i][j] * F;
	return newMat;
}
const Mat2 & operator*=(Mat2 & M1, const float & F)
{
	int i, j;
	for (i = 0; i < 2; i++) for (j = 0; j < 2; j++) M1[i][j] *= F;
	return M1;
}
const Vec2 operator*(const Mat2 & M1, const Vec2 & V)
{
	return Vec2(M1[0][0] * V.x + M1[0][1] * V.y,
				M1[1][0] * V.x + M1[1][1] * V.y);
}

const bool operator==(const Mat2 & M1, const Mat2 & M2)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (std::abs(M1[i][j] - M2[i][j]) > TOLERANCE) return false;
		}
	}
	return true;
}

const bool operator!=(const Mat2 & M1, const Mat2 & M2)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (std::abs(M1[i][j] - M2[i][j]) > TOLERANCE) return true;
		}
	}
	return false;
}

std::string Mat2::ToString() const
{
	std::ostringstream oss;
	oss << "| " << values[0][0] << " " << values[0][1] << " |" << std::endl;
	oss << "| " << values[1][0] << " " << values[1][1] << " |" << std::endl;
	return oss.str();
}

std::ostream & operator<<(std::ostream & os, const Mat2 & M)
{
	os << M.ToString();
	return os;
}

std::istream & operator>>(std::istream & is, Mat2 & M)
{
	is >> M[0][0] >> M[0][1] >> M[1][0] >> M[1][1];
	return is;
}

void Mat2::Transpose()
{
	values = Transposed(*this).values;
}

const Mat2 Transposed(const Mat2& M)
{
	return Mat2({
					{
					{M[0][0], M[1][0]},
					{M[0][1], M[1][1]}
					}
				});
}

float Mat2::Determinant() const
{
	return (values[0][0] * values[1][1] - values[0][1] * values[1][0]);
}

const Mat2 Inversed(const Mat2& M)
{
	return Mat2();
	float Det = M.Determinant();
	if (Det == 0.f) throw std::overflow_error("Division by 0!");
	float InvertedDeterminant = 1 / Det;
	return InvertedDeterminant * Mat2({ 
										{
										{M[1][1], -M[0][1]},
										{-M[1][0], M[0][0]}
										} 
									  });
}

void Mat2::Inverse()
{
	values = Inversed(*this).values;
}

void Mat2::Clean()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (std::abs(values[i][j]) <= TOLERANCE) values[i][j] = 0.f;
		}
	}
}

float * Mat2::GetData() const
{
	float* arr = new float[4]{ values[0][0], values[1][0], values[0][1], values[1][1] };
	return arr;
}

Mat2 Mat2::IdentityMat()
{
	return Mat2({
					{
					{1, 0},
					{0, 1}
					}
				});
}

Mat2 Mat2::ScaleMat(float scalar)
{
	return Mat2({
					{
					{scalar, 0},
					{0, scalar}
					}
				});
}

Mat2 Mat2::RotationMat(const float Degrees)
{
	return Mat2({
					{
					{std::cos((Degrees*PI) / 180), -std::sin((Degrees*PI) / 180)},
					{std::sin((Degrees*PI) / 180), std::cos((Degrees*PI) / 180)}
					}
				});
}


/////////
// Mat3
/////////

Mat3::Mat3()
{

}

Mat3::Mat3(std::array<std::array<float, 3>, 3> newValues)
{
	values = newValues;
}

Mat3::Mat3(const Mat2 & newMat)
{
	values[0][0] = newMat[0][0];
	values[0][1] = newMat[0][2];
	values[1][0] = newMat[1][0];
	values[1][1] = newMat[1][1];
}

Mat3::Mat3(const Mat3 & newMat)
{
	values = newMat.values;
}

Mat3::Mat3(const Mat4 & newMat)
{
	int i, j;
	for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) values[i][j] = newMat[i][j];
}

std::array<float, 3>& Mat3::operator[](int row)
{
	return values[row];
}

std::array<float, 3> Mat3::operator[](int row) const
{
	return values[row];
}

const Mat3 operator+(const Mat3 & M1, const Mat3 & M2)
{
	return Mat3(
		{
		{
			{M1[0][0] + M2[0][0], M1[0][1] + M2[0][1], M1[0][2] + M2[0][2]},
			{M1[1][0] + M2[1][0], M1[1][1] + M2[1][1], M1[1][2] + M2[1][2]},
			{M1[2][0] + M2[2][0], M1[2][1] + M2[2][1], M1[2][2] + M2[2][2]}
		}
		}
	);
}
const Mat3 operator-(const Mat3 & M1, const Mat3 & M2)
{
	return Mat3(
		{
		{
			{M1[0][0] - M2[0][0], M1[0][1] - M2[0][1], M1[0][2] - M2[0][2]},
			{M1[1][0] - M2[1][0], M1[1][1] - M2[1][1], M1[1][2] - M2[1][2]},
			{M1[2][0] - M2[2][0], M1[2][1] - M2[2][1], M1[2][2] - M2[2][2]}
		}
		}
	);
}
const Mat3 operator*(const Mat3 & M1, const Mat3 & M2)
{
	return Mat3(
		{
		{
			{
			M1[0][0] * M2[0][0] + M1[0][1] * M2[1][0] + M1[0][2] * M2[2][0],
			M1[0][0] * M2[0][1] + M1[0][1] * M2[1][1] + M1[0][2] * M2[2][1],
			M1[0][0] * M2[0][2] + M1[0][1] * M2[1][2] + M1[0][2] * M2[2][2]
			},
			{
			M1[1][0] * M2[0][0] + M1[1][1] * M2[1][0] + M1[1][2] * M2[2][0],
			M1[1][0] * M2[0][1] + M1[1][1] * M2[1][1] + M1[1][2] * M2[2][1],
			M1[1][0] * M2[0][2] + M1[1][1] * M2[1][2] + M1[1][2] * M2[2][2]
			},
			{
			M1[2][0] * M2[0][0] + M1[2][1] * M2[1][0] + M1[2][2] * M2[2][0],
			M1[2][0] * M2[0][1] + M1[2][1] * M2[1][1] + M1[2][2] * M2[2][1],
			M1[2][0] * M2[0][2] + M1[2][1] * M2[1][2] + M1[2][2] * M2[2][2]
			}
		}
		}
	);
}
Mat3& Mat3::operator+=(const Mat3& M)
{
	int i, j;
	for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) values[i][j] += M[i][j];
	return *this;
}
Mat3& Mat3::operator-=(const Mat3& M)
{
	int i, j;
	for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) values[i][j] -= M[i][j];
	return *this;
}
Mat3& Mat3::operator*=(const Mat3& M)
{
	values[0][0] = values[0][0] * M[0][0] + values[0][1] * M[1][0] + values[0][2] * M[2][0];
	values[0][1] = values[0][0] * M[0][1] + values[0][1] * M[1][1] + values[0][2] * M[2][1];
	values[0][2] = values[0][0] * M[0][2] + values[0][1] * M[1][2] + values[0][2] * M[2][2];
	values[1][0] = values[1][0] * M[0][0] + values[1][1] * M[1][0] + values[1][2] * M[2][0];
	values[1][1] = values[1][0] * M[0][1] + values[1][1] * M[1][1] + values[1][2] * M[2][1];
	values[1][2] = values[1][0] * M[0][2] + values[1][1] * M[1][2] + values[1][2] * M[2][2];
	values[2][0] = values[2][0] * M[0][0] + values[2][1] * M[1][0] + values[2][2] * M[2][0];
	values[2][1] = values[2][0] * M[0][1] + values[2][1] * M[1][1] + values[2][2] * M[2][1];
	values[2][2] = values[2][0] * M[0][2] + values[2][1] * M[1][2] + values[2][2] * M[2][2];
	return *this;
}
const Mat3 operator*(const Mat3 & M1, const float & F)
{
	Mat3 newMat = Mat3();
	int i, j;
	for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) newMat[i][j] = M1[i][j] * F;
	return newMat;
}
const Mat3 operator*(const float & F, const Mat3 & M1)
{
	Mat3 newMat = Mat3();
	int i, j;
	for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) newMat[i][j] = M1[i][j] * F;
	return newMat;
}
const Mat3 & operator*=(Mat3 & M1, const float & F)
{
	int i, j;
	for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) M1[i][j] *= F;
	return M1;
}
const Vec3 operator*(const Mat3 & M1, const Vec3 & V)
{
	return Vec3(M1[0][0] * V.x + M1[0][1] * V.y + M1[0][2] * V.z,
				M1[1][0] * V.x + M1[1][1] * V.y + M1[1][2] * V.z,
				M1[2][0] * V.x + M1[2][1] * V.y + M1[2][2] * V.z);
}

const bool operator==(const Mat3 & M1, const Mat3 & M2)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (std::abs(M1[i][j] - M2[i][j]) > TOLERANCE) return false;
		}
	}
	return true;
}

const bool operator!=(const Mat3 & M1, const Mat3 & M2)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (std::abs(M1[i][j] - M2[i][j]) > TOLERANCE) return true;
		}
	}
	return false;
}

std::string Mat3::ToString() const
{
	std::ostringstream oss;
	oss << "| " << values[0][0] << " " << values[0][1] << " " << values[0][2] << " |" << std::endl;
	oss << "| " << values[1][0] << " " << values[1][1] << " " << values[1][2] << " |" << std::endl;
	oss << "| " << values[2][0] << " " << values[2][1] << " " << values[2][2] << " |" << std::endl;
	return oss.str();
}

std::ostream & operator<<(std::ostream & os, const Mat3 & M)
{
	os << M.ToString();
	return os;
}

std::istream & operator>>(std::istream & is, Mat3 & M)
{
	is >> M[0][0] >> M[0][1] >> M[0][2] >> M[1][0] >> M[1][1] >> M[1][2] >> M[2][0] >> M[2][1] >> M[2][2];
	return is;
}

void Mat3::Transpose()
{
	values = Transposed(*this).values;
}

const Mat3 Transposed(const Mat3& M)
{
	return Mat3(
		{
		{
			{M[0][0], M[1][0], M[2][0]},
			{M[0][1], M[1][1], M[2][1]},
			{M[0][2], M[1][2], M[2][2]}
		}
		}
	);
}

float Mat3::Determinant() const
{
	return (values[0][0] * values[1][1] * values[2][2] +
			values[0][1] * values[1][2] * values[2][0] +
			values[0][2] * values[1][0] * values[2][1] -
			values[0][2] * values[1][1] * values[2][0] -
			values[0][1] * values[1][0] * values[2][2] -
			values[0][0] * values[1][2] * values[2][1]
			);
}

void Mat3::Inverse()
{
	values = Inversed(*this).values;
}

void Mat3::Clean()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (std::abs(values[i][j]) <= TOLERANCE) values[i][j] = 0.f;
		}
	}
}

const Mat3 Inversed(const Mat3& M)
{
	float Det = M.Determinant();
	if (Det == 0.f) throw std::overflow_error("Division by 0!");
	Mat3 determinantMat = Mat3( {	{
									{ 0.f, 0.f, 0.f },
									{ 0.f, 0.f, 0.f },
									{ 0.f, 0.f, 0.f }
									}
									});

	determinantMat[0][0] = Mat2({ {{M[1][1], M[1][2]}, {M[2][1], M[2][2]} } }).Determinant();
	determinantMat[0][1] = Mat2({ {{M[1][0], M[1][2]}, {M[2][0], M[2][2]} } }).Determinant();
	determinantMat[0][2] = Mat2({ {{M[1][0], M[1][1]}, {M[2][0], M[2][1]} } }).Determinant();
	determinantMat[1][0] = Mat2({ {{M[0][1], M[0][2]}, {M[2][1], M[2][2]} } }).Determinant();
	determinantMat[1][1] = Mat2({ {{M[0][0], M[0][2]}, {M[2][0], M[2][2]} } }).Determinant();
	determinantMat[1][2] = Mat2({ {{M[0][0], M[0][1]}, {M[2][0], M[2][1]} } }).Determinant();
	determinantMat[2][0] = Mat2({ {{M[0][1], M[0][2]}, {M[1][1], M[1][2]} } }).Determinant();
	determinantMat[2][1] = Mat2({ {{M[0][0], M[0][2]}, {M[1][0], M[1][2]} } }).Determinant();
	determinantMat[2][2] = Mat2({ {{M[0][0], M[0][1]}, {M[1][0], M[1][1]} } }).Determinant();

	determinantMat[0][1] = -determinantMat[0][1];
	determinantMat[1][0] = -determinantMat[1][0];
	determinantMat[1][2] = -determinantMat[1][2];
	determinantMat[2][1] = -determinantMat[2][1];

	determinantMat.Transpose();
	determinantMat *= (1/Det);

	return determinantMat;
}

float* Mat3::GetData() const
{
	float* arr = new float[9]{ values[0][0], values[1][0], values[2][0], values[0][1], values[1][1], values[2][1], values[0][2], values[1][2], values[2][2] };
	return arr;
}

Mat3 Mat3::IdentityMat()
{
	return Mat3(
		{
		{
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1}
		}
		}
	);
}

Mat3 Mat3::GetDual(const Vec3 & V)
{
	return Mat3(
		{ {
			{0, -V.z, V.y},
			{V.z, 0, -V.x},
			{-V.y, V.x, 0}
		} }
	);
}

Mat3 Mat3::TranslationMat(const struct Vec2& V)
{
	return Mat3(
		{
		{
			{1, 0, V.x},
			{0, 1, V.y},
			{0, 0, 1}
		}
		}
	);
}

Mat3 Mat3::ScaleMat(const Vec3 V)
{
	return Mat3(
		{
		{
			{V.x, 0, 0},
			{0, V.y, 0},
			{0, 0, V.z}
		}
		}
	);
}

Mat3 Mat3::RotationMat(const Vec3& V, const float Degrees)
{
	Mat3 AMat = GetDual(V);
	return (IdentityMat() + std::sin((Degrees*PI)/180) * AMat + (1 - std::cos((Degrees*PI) / 180)) * (AMat*AMat));
}


/////////
// Mat4
/////////

Mat4::Mat4()
{

}

Mat4::Mat4(std::array<std::array<float, 4>, 4> newValues)
{
	values = newValues;
}

Mat4::Mat4(const Mat2 & newMat)
{
	values[0][0] = newMat[0][0];
	values[0][1] = newMat[0][2];
	values[1][0] = newMat[1][0];
	values[1][1] = newMat[1][1];
}

Mat4::Mat4(const Mat3 & newMat)
{
	values[0][0] = newMat[0][0];
	values[0][1] = newMat[0][1];
	values[0][2] = newMat[0][2];
	values[1][0] = newMat[1][0];
	values[1][1] = newMat[1][1];
	values[1][2] = newMat[1][2];
	values[2][0] = newMat[2][0];
	values[2][1] = newMat[2][1];
	values[2][2] = newMat[2][2];
	values[3][3] = 1.0f;
}

Mat4::Mat4(const Mat4 & newMat)
{
	values = newMat.values;
}

std::array<float, 4>& Mat4::operator[](int row)
{
	return values[row];
}

std::array<float, 4> Mat4::operator[](int row) const
{
	return values[row];
}

const Mat4 operator+(const Mat4 & M1, const Mat4 & M2)
{
	return Mat4(
		{
		{
			{M1[0][0] + M2[0][0], M1[0][1] + M2[0][1], M1[0][2] + M2[0][2], M1[0][3] + M2[0][3]},
			{M1[1][0] + M2[1][0], M1[1][1] + M2[1][1], M1[1][2] + M2[1][2], M1[1][3] + M2[1][3]},
			{M1[2][0] + M2[2][0], M1[2][1] + M2[2][1], M1[2][2] + M2[2][2], M1[2][3] + M2[2][3]},
			{M1[3][0] + M2[3][0], M1[3][1] + M2[3][1], M1[3][2] + M2[3][2], M1[3][3] + M2[3][3]}
		}
		}
	);
}
const Mat4 operator-(const Mat4 & M1, const Mat4 & M2)
{
	return Mat4(
		{
		{
			{M1[0][0] - M2[0][0], M1[0][1] - M2[0][1], M1[0][2] - M2[0][2], M1[0][3] - M2[0][3]},
			{M1[1][0] - M2[1][0], M1[1][1] - M2[1][1], M1[1][2] - M2[1][2], M1[1][3] - M2[1][3]},
			{M1[2][0] - M2[2][0], M1[2][1] - M2[2][1], M1[2][2] - M2[2][2], M1[2][3] - M2[2][3]},
			{M1[3][0] - M2[3][0], M1[3][1] - M2[3][1], M1[3][2] - M2[3][2], M1[3][3] - M2[3][3]}
		}
		}
	);
}
const Mat4 operator*(const Mat4 & M1, const Mat4 & M2)
{
	return Mat4(
		{
		{
			{
			M1[0][0] * M2[0][0] + M1[0][1] * M2[1][0] + M1[0][2] * M2[2][0] + M1[0][3] * M2[3][0],
			M1[0][0] * M2[0][1] + M1[0][1] * M2[1][1] + M1[0][2] * M2[2][1] + M1[0][3] * M2[3][1],
			M1[0][0] * M2[0][2] + M1[0][1] * M2[1][2] + M1[0][2] * M2[2][2] + M1[0][3] * M2[3][2],
			M1[0][0] * M2[0][3] + M1[0][1] * M2[1][3] + M1[0][2] * M2[2][3] + M1[0][3] * M2[3][3]
			},
			{
			M1[1][0] * M2[0][0] + M1[1][1] * M2[1][0] + M1[1][2] * M2[2][0] + M1[1][3] * M2[3][0],
			M1[1][0] * M2[0][1] + M1[1][1] * M2[1][1] + M1[1][2] * M2[2][1] + M1[1][3] * M2[3][1],
			M1[1][0] * M2[0][2] + M1[1][1] * M2[1][2] + M1[1][2] * M2[2][2] + M1[1][3] * M2[3][2],
			M1[1][0] * M2[0][3] + M1[1][1] * M2[1][3] + M1[1][2] * M2[2][3] + M1[1][3] * M2[3][3]
			},
			{
			M1[2][0] * M2[0][0] + M1[2][1] * M2[1][0] + M1[2][2] * M2[2][0] + M1[2][3] * M2[3][0],
			M1[2][0] * M2[0][1] + M1[2][1] * M2[1][1] + M1[2][2] * M2[2][1] + M1[2][3] * M2[3][1],
			M1[2][0] * M2[0][2] + M1[2][1] * M2[1][2] + M1[2][2] * M2[2][2] + M1[2][3] * M2[3][2],
			M1[2][0] * M2[0][3] + M1[2][1] * M2[1][3] + M1[2][2] * M2[2][3] + M1[2][3] * M2[3][3]
			},
			{
			M1[3][0] * M2[0][0] + M1[3][1] * M2[1][0] + M1[3][2] * M2[2][0] + M1[3][3] * M2[3][0],
			M1[3][0] * M2[0][1] + M1[3][1] * M2[1][1] + M1[3][2] * M2[2][1] + M1[3][3] * M2[3][1],
			M1[3][0] * M2[0][2] + M1[3][1] * M2[1][2] + M1[3][2] * M2[2][2] + M1[3][3] * M2[3][2],
			M1[3][0] * M2[0][3] + M1[3][1] * M2[1][3] + M1[3][2] * M2[2][3] + M1[3][3] * M2[3][3]
			}
			}
		}
	);
}
Mat4& Mat4::operator+=(const Mat4& M)
{
	int i, j;
	for (i = 0; i < 4; i++) for (j = 0; j < 4; j++) values[i][j] += M[i][j];
	return *this;
}
Mat4& Mat4::operator-=(const Mat4& M)
{
	int i, j;
	for (i = 0; i < 4; i++) for (j = 0; j < 4; j++) values[i][j] -= M[i][j];
	return *this;
}
Mat4& Mat4::operator*=(const Mat4& M)
{
	values[0][0] = values[0][0] * M[0][0] + values[0][1] * M[1][0] + values[0][2] * M[2][0] + values[0][3] * M[3][0];
	values[0][1] = values[0][0] * M[0][1] + values[0][1] * M[1][1] + values[0][2] * M[2][1] + values[0][3] * M[3][1];
	values[0][2] = values[0][0] * M[0][2] + values[0][1] * M[1][2] + values[0][2] * M[2][2] + values[0][3] * M[3][2];
	values[0][3] = values[0][0] * M[0][2] + values[0][1] * M[1][2] + values[0][2] * M[2][2] + values[0][3] * M[3][3];
	values[1][0] = values[1][0] * M[0][0] + values[1][1] * M[1][0] + values[1][2] * M[2][0] + values[1][3] * M[3][0];
	values[1][1] = values[1][0] * M[0][1] + values[1][1] * M[1][1] + values[1][2] * M[2][1] + values[1][3] * M[3][1];
	values[1][2] = values[1][0] * M[0][2] + values[1][1] * M[1][2] + values[1][2] * M[2][2] + values[1][3] * M[3][2];
	values[1][3] = values[1][0] * M[0][3] + values[1][1] * M[1][3] + values[1][2] * M[2][3] + values[1][3] * M[3][3];
	values[2][0] = values[2][0] * M[0][0] + values[2][1] * M[1][0] + values[2][2] * M[2][0] + values[2][3] * M[3][0];
	values[2][1] = values[2][0] * M[0][1] + values[2][1] * M[1][1] + values[2][2] * M[2][1] + values[2][3] * M[3][1];
	values[2][2] = values[2][0] * M[0][2] + values[2][1] * M[1][2] + values[2][2] * M[2][2] + values[2][3] * M[3][2];
	values[2][3] = values[2][0] * M[0][2] + values[2][1] * M[1][2] + values[2][2] * M[2][2] + values[2][3] * M[3][3];
	values[3][0] = values[3][0] * M[0][0] + values[3][1] * M[1][0] + values[3][2] * M[2][0] + values[3][3] * M[3][0];
	values[3][1] = values[3][0] * M[0][1] + values[3][1] * M[1][1] + values[3][2] * M[2][1] + values[3][3] * M[3][1];
	values[3][2] = values[3][0] * M[0][2] + values[3][1] * M[1][2] + values[3][2] * M[2][2] + values[3][3] * M[3][2];
	values[3][3] = values[3][0] * M[0][2] + values[3][1] * M[1][2] + values[3][2] * M[2][2] + values[3][3] * M[3][3];
	return *this;
}
const Mat4 operator*(const Mat4 & M1, const float & F)
{
	Mat4 newMat = Mat4();
	int i, j;
	for (i = 0; i < 4; i++) for (j = 0; j < 4; j++) newMat[i][j] = M1[i][j] * F;
	return newMat;
}
const Mat4 operator*(const float & F, const Mat4 & M1)
{
	Mat4 newMat = Mat4();
	int i, j;
	for (i = 0; i < 4; i++) for (j = 0; j < 4; j++) newMat[i][j] = M1[i][j] * F;
	return newMat;
}
const Mat4 & operator*=(Mat4 & M1, const float & F)
{
	int i, j;
	for (i = 0; i < 4; i++) for (j = 0; j < 4; j++) M1[i][j] *= F;
	return M1;
}
const Vec4 operator*(const Mat4 & M1, const Vec4 & V)
{
	return Vec4(M1[0][0] * V.x + M1[0][1] * V.y + M1[0][2] * V.z + M1[0][3] * V.w,
				M1[1][0] * V.x + M1[1][1] * V.y + M1[1][2] * V.z + M1[1][3] * V.w,
				M1[2][0] * V.x + M1[2][1] * V.y + M1[2][2] * V.z + M1[2][3] * V.w,
				M1[3][0] * V.x + M1[3][1] * V.y + M1[3][2] * V.z + M1[3][3] * V.w);
}

const bool operator==(const Mat4 & M1, const Mat4 & M2)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (std::abs(M1[i][j] - M2[i][j]) > TOLERANCE) return false;
		}
	}
	return true;
}

const bool operator!=(const Mat4 & M1, const Mat4 & M2)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (std::abs(M1[i][j] - M2[i][j]) > TOLERANCE) return true;
		}
	}
	return false;
}

std::string Mat4::ToString() const
{
	std::ostringstream oss;
	oss << "| " << values[0][0] << " " << values[0][1] << " " << values[0][2] << " " << values[0][3] << " |" << std::endl;
	oss << "| " << values[1][0] << " " << values[1][1] << " " << values[1][2] << " " << values[1][3] << " |" << std::endl;
	oss << "| " << values[2][0] << " " << values[2][1] << " " << values[2][2] << " " << values[2][3] << " |" << std::endl;
	oss << "| " << values[3][0] << " " << values[3][1] << " " << values[3][2] << " " << values[3][3] << " |" << std::endl;
	return oss.str();
}

std::ostream & operator<<(std::ostream & os, const Mat4 & M)
{
	os << M.ToString();
	return os;
}

std::istream & operator>>(std::istream & is, Mat4 & M)
{
	is >> M[0][0] >> M[0][1] >> M[0][2] >> M[0][3] >> M[1][0] >> M[1][1] >> M[1][2] >> M[1][3] >> M[2][0] >> M[2][1] >> M[2][2] >> M[2][3] >> M[3][0] >> M[3][1] >> M[3][2] >> M[3][3];
	return is;
}

void Mat4::Transpose()
{
	values = Transposed(*this).values;
}

const Mat4 Transposed(const Mat4& M)
{
	return Mat4(
		{
		{
			{M[0][0], M[1][0], M[2][0], M[3][0]},
			{M[0][1], M[1][1], M[2][1], M[3][1]},
			{M[0][2], M[1][2], M[2][2], M[3][2]},
			{M[0][3], M[1][3], M[2][3], M[3][3]}
		}
		}
	);
}

float Mat4::Determinant() const
{
	return (values[0][0] * values[1][1] * values[2][2] +
			values[0][1] * values[1][2] * values[2][0] +
			values[0][2] * values[1][0] * values[2][1] -
			values[0][2] * values[1][1] * values[2][0] -
			values[0][1] * values[1][0] * values[2][2] -
			values[0][0] * values[1][2] * values[2][1]
			);
}

void Mat4::Clean()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (std::abs(values[i][j]) <= TOLERANCE) values[i][j] = 0.f;
		}
	}
}

float* Mat4::GetData() const
{
	float* arr = new float[16]{ values[0][0], values[1][0], values[2][0], values[3][0], values[0][1], values[1][1], values[2][1], values[3][1], values[0][2], values[1][2], values[2][2], values[3][2], values[0][3], values[1][3], values[2][3], values[3][3] };
	return arr;
}

Mat4 Mat4::IdentityMat()
{
	return Mat4(
		{
		{
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}
		}
		}
	);
}

Mat4 Mat4::TranslationMat(const struct Vec3& V)
{
	return Mat4(
		{
		{
			{1, 0, 0, V.x},
			{0, 1, 0, V.y},
			{0, 0, 1, V.z},
			{0, 0, 0, 1}
		}
		}
	);
}

Mat4 Mat4::ScaleMat(const Vec3 V)
{
	return Mat4(
		{
		{
			{V.x, 0, 0, 0},
			{0, V.y, 0, 0},
			{0, 0, V.z, 0},
			{0, 0, 0, 1}
		}
		}
	);
}

Mat4 Mat4::RotationMat(Vec4 V, const float Degrees)
{
	Mat4 RotMat = Mat4(Mat3::RotationMat(V, Degrees));
	RotMat[3][3] = 1;
	return RotMat;
}

Mat4 Mat4::ViewMat(const Vec3 center, const Vec3 eye, const Vec3 up)
{
	return ViewMat(center - eye, up);
}

Mat4 Mat4::ViewMat(const Vec3 direction, const Vec3 up)
{
	Vec3 V = Normalized(direction);
	Vec3 S = Normalized(Cross(V, up));
	Vec3 U = Cross(S, V);
	return Mat4(
		{
		{
			{S.x, S.y, S.z, 0},
			{U.x, U.y, U.z, 0},
			{-V.x, -V.y, -V.z, 0},
			{0, 0, 0, 1}
		}
		}
	);
}

Mat4 Mat4::ViewMatrix(const Vec3 V, const Vec3 U, const Vec3 S, const Vec3 pos)
{
	float dotSide = Dot(S, pos);
	float dotUp = Dot(U, pos);
	float dotView = Dot(V, pos);

	return Mat4(
		{
		{
			{S.x, S.y, S.z, -dotSide},
			{U.x, U.y, U.z, -dotUp},
			{-V.x, -V.y, -V.z, dotView},
			{0, 0, 0, 1}
		}
		}
	);

}

Mat4 Mat4::OrthographicMat(float n, float f, float b, float t, float r, float l)
{
	return Mat4(
		{
		{
			{2/(r-l), 0, 0, (l+r)/(l-r)},
			{0, 2/(t-b), 0, (b+t)/(b-t)},
			{0, 0, 2/(f-n), (n+r)/(n-f)},
			{0, 0, 0, 1}
		}
		}
	);
}

Mat4 Mat4::ProjectionMat(float fov, float aspect, float n, float f)
{
	float degrees = fov / 2;
	float d = 1 / std::tan(degrees);
	return Mat4(
		{
		{
			{d/aspect, 0, 0, 0},
			{0, d, 0, 0},
			{0, 0, (n+f)/(n-f), (2*f*n)/(n-f)},
			{0, 0, -1, 0}
		}
		}
	);
}
