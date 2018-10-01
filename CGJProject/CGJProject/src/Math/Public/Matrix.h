#pragma once
#include <iostream>
#include <string.h>
#include <array>

enum class EAxis { X, Y, Z};

class Mat2
{

};

class Mat3
{

public:

	std::array<std::array<float, 3>, 3> values = { {
													{ 0.f, 0.f, 0.f },
													{ 0.f, 0.f, 0.f },
													{ 0.f, 0.f, 0.f }
													}
												 };
	Mat3();
	Mat3(std::array<std::array<float, 3>, 3> newValues);
	Mat3(const Mat3& newMat);

	std::array<float, 3>& operator[](int row);
	std::array<float, 3> operator[](int row) const;

	friend const Mat3 operator+(const Mat3& M1, const Mat3& M2);
	friend const Mat3& operator+=(Mat3& M1, const Mat3& M2);
	friend const Mat3 operator-(const Mat3& M1, const Mat3& M2);
	friend const Mat3& operator-=(Mat3& M1, const Mat3& M2);
	friend const Mat3 operator*(const Mat3& M1, const Mat3& M2);
	friend const Mat3& operator*=(Mat3& M1, const Mat3& M2);
	friend const Mat3 operator*(const Mat3& M1, const float& F);
	friend const Mat3 operator*(const float& F, const Mat3& M1);
	friend const Mat3& operator*=(Mat3& M1, const float& F);
	friend const class Vec3 operator*(const Mat3& M1, const class Vec3& V);
	friend const bool operator==(const Mat3& M1, const Mat3& M2);
	friend const bool operator!=(const Mat3& M1, const Mat3& M2);
	std::string ToString() const;
	friend std::ostream& operator<<(std::ostream& os, const Mat3& M);
	friend std::istream& operator>>(std::istream& is, Mat3& M);

	std::array<std::array<float, 3>, 3> GetValues() const;
	void Transpose();
	Mat3 GetTransposed() const;
	float Determinant() const;
	Mat3 Inverse() const;

	std::array<float, 9> GetData() const;

	static Mat3 IdentityMat();
	static Mat3 TranslationMat(float x, float y);
	static Mat3 ScaleMat(float scalar);
	static Mat3 RotationMat(EAxis RotationAxis, float Degrees);

};

class Mat4
{

};