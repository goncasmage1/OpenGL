#pragma once
#include <iostream>
#include <string.h>
#include <array>

enum class EAxis { X, Y, Z};

struct Mat2
{
	std::array<std::array<float, 2>, 2> values = { {
													{ 0.f, 0.f},
													{ 0.f, 0.f}
													}
	};
	Mat2();
	Mat2(std::array<std::array<float, 2>, 2> newValues);
	Mat2(const Mat2& newMat);
	Mat2(const struct Mat3& newMat);

	std::array<float, 2>& operator[](int row);
	std::array<float, 2> operator[](int row) const;

	friend const Mat2 operator+(const Mat2& M1, const Mat2& M2);
	friend const Mat2 operator-(const Mat2& M1, const Mat2& M2);
	friend const Mat2 operator*(const Mat2& M1, const Mat2& M2);
	Mat2& operator+=(const Mat2& M);
	Mat2& operator-=(const Mat2& M);
	Mat2& operator*=(const Mat2& M);
	friend const Mat2 operator*(const Mat2& M1, const float& F);
	friend const Mat2 operator*(const float& F, const Mat2& M1);
	friend const Mat2& operator*=(Mat2& M1, const float& F);
	friend const struct Vec2 operator*(const Mat2& M1, const struct Vec2& V);
	friend const bool operator==(const Mat2& M1, const Mat2& M2);
	friend const bool operator!=(const Mat2& M1, const Mat2& M2);
	std::string ToString() const;
	friend std::ostream& operator<<(std::ostream& os, const Mat2& M);
	friend std::istream& operator>>(std::istream& is, Mat2& M);

	void Transpose();
	Mat2 GetTransposed() const;
	float Determinant() const;
	Mat2 GetInverse() const;

	float* GetData() const;

	static Mat2 IdentityMat();
	//static Mat2 TranslationMat(float x, float y);
	static Mat2 ScaleMat(float scalar);
	static Mat2 RotationMat(EAxis RotationAxis, double Degrees);
};

struct Mat3
{
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
	friend const Mat3 operator-(const Mat3& M1, const Mat3& M2);
	friend const Mat3 operator*(const Mat3& M1, const Mat3& M2);
	Mat3& operator+=(const Mat3& M);
	Mat3& operator-=(const Mat3& M);
	Mat3& operator*=(const Mat3& M);
	friend const Mat3 operator*(const Mat3& M1, const float& F);
	friend const Mat3 operator*(const float& F, const Mat3& M1);
	friend const Mat3& operator*=(Mat3& M1, const float& F);
	friend const struct Vec3 operator*(const Mat3& M1, const struct Vec3& V);
	friend const bool operator==(const Mat3& M1, const Mat3& M2);
	friend const bool operator!=(const Mat3& M1, const Mat3& M2);
	std::string ToString() const;
	friend std::ostream& operator<<(std::ostream& os, const Mat3& M);
	friend std::istream& operator>>(std::istream& is, Mat3& M);

	void Transpose();
	Mat3 GetTransposed() const;
	float Determinant() const;
	Mat3 GetInverse() const;

	float* GetData() const;

	static Mat3 IdentityMat();
	static Mat3 TranslationMat(float x, float y);
	static Mat3 ScaleMat(float scalar);
	static Mat3 RotationMat(EAxis RotationAxis, double Degrees);

};

struct Mat4
{

};