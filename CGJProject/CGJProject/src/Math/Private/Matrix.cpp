#include "../Public/Matrix.h"
#include "../Public/Vector.h"
#include <sstream>
#include <math.h>
#include <assert.h>

#define PI 3.14159265358979323846

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

std::array<float, 2>& Mat2::operator[](int row)
{
	return values[row];
}

std::array<float, 2> Mat2::operator[](int row) const
{
	return values[row];
}

void Mat2::Transpose()
{
	values = GetTransposed().values;
}

Mat2 Mat2::GetTransposed() const
{
	return Mat2({
					{
					{values[0][0], values[1][0]},
					{values[0][1], values[1][1]}
					}
				});
}

float Mat2::Determinant() const
{
	return (values[0][0] * values[1][1] - values[0][1] * values[1][0]);
}

Mat2 Mat2::GetInverse() const
{
	float InvertedDeterminant = 1 / Determinant();
	return InvertedDeterminant * Mat2({ 
										{
										{values[1][1], -values[0][1]},
										{-values[1][0], values[0][0]}
										} 
									  });
}

float * Mat2::GetData() const
{
	float arr[4] = { values[0][0], values[1][0], values[0][1], values[1][1] };
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

Mat3::Mat3(const Mat3 & newMat)
{
	values = newMat.values;
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
	return Mat3(
		{
		{
			{M1[0][0] * F, M1[0][1] * F, M1[0][2] * F},
			{M1[1][0] * F, M1[1][1] * F, M1[1][2] * F},
			{M1[2][0] * F, M1[2][1] * F, M1[2][2] * F}
		}
		}
	);
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
	return (M1[0] == M2[0] &&
			M1[1] == M2[1] &&
			M1[2] == M2[2]
			);
}

const bool operator!=(const Mat3 & M1, const Mat3 & M2)
{
	return (M1[0] != M2[0] ||
			M1[1] != M2[1] ||
			M1[2] != M2[2]
			);
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
	values = GetTransposed().values;
}

Mat3 Mat3::GetTransposed() const
{
	return Mat3(
		{
		{
			{values[0][0], values[1][0], values[2][0]},
			{values[0][1], values[1][1], values[2][1]},
			{values[0][2], values[1][2], values[2][2]}
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

Mat3 Mat3::GetInverse() const
{
	Mat3 Transposed = GetTransposed();
	float Det = Determinant();

	std::array<std::array<float, 3>, 3> determinants = { {
													{ 0.f, 0.f, 0.f },
													{ 0.f, 0.f, 0.f },
													{ 0.f, 0.f, 0.f }
													}
	};

	determinants[0][0] = Mat2({ {{values[1][1], values[1][2]}, {values[2][1], values[2][2]} } }).Determinant();
	determinants[0][0] = Mat2({ {{values[1][1], values[1][2]}, {values[2][1], values[2][2]} } }).Determinant();
	determinants[0][0] = Mat2({ {{values[1][1], values[1][2]}, {values[2][1], values[2][2]} } }).Determinant();
	determinants[0][0] = Mat2({ {{values[1][1], values[1][2]}, {values[2][1], values[2][2]} } }).Determinant();
	determinants[0][0] = Mat2({ {{values[1][1], values[1][2]}, {values[2][1], values[2][2]} } }).Determinant();
	determinants[0][0] = Mat2({ {{values[1][1], values[1][2]}, {values[2][1], values[2][2]} } }).Determinant();
	determinants[0][0] = Mat2({ {{values[1][1], values[1][2]}, {values[2][1], values[2][2]} } }).Determinant();
	determinants[0][0] = Mat2({ {{values[1][1], values[1][2]}, {values[2][1], values[2][2]} } }).Determinant();
	determinants[0][0] = Mat2({ {{values[1][1], values[1][2]}, {values[2][1], values[2][2]} } }).Determinant();

	return Mat3();
}

float* Mat3::GetData() const
{
	float arr[9] = { values[0][0], values[1][0], values[2][0], values[0][1], values[1][1], values[2][1], values[0][2], values[1][2], values[2][2] };
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

Mat3 Mat3::TranslationMat(float x, float y)
{
	return Mat3(
		{
		{
			{1, 0, x},
			{0, 1, y},
			{0, 0, 1}
		}
		}
	);
}

Mat3 Mat3::ScaleMat(float scalar)
{
	return Mat3(
		{
		{
			{scalar, 0, 0},
			{0, scalar, 0},
			{0, 0, scalar}
		}
		}
	);
}

Mat3 Mat3::RotationMat(EAxis RotationAxis, double Degrees)
{
	
}


