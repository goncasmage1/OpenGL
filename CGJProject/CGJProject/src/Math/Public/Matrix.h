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
	Mat2(const struct Mat4& newMat);

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
	friend const Mat2 Transposed(const Mat2& M);
	float Determinant() const;
	friend const Mat2 Inversed(const Mat2& M);
	void Inverse();

	float* GetData() const;

	static Mat2 IdentityMat();
	static Mat2 ScaleMat(float scalar);
	static Mat2 RotationMat(const float Degrees);
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
	Mat3(const struct Mat2& newMat);
	Mat3(const Mat3& newMat);
	Mat3(const struct Mat4& newMat);

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
	friend const Mat3 Transposed(const Mat3& M);
	float Determinant() const;
	friend const Mat3 Inversed(const Mat3& M);
	void Inverse();

	float* GetData() const;

	static Mat3 IdentityMat();
	static Mat3 GetDual(const struct Vec3& V);
	static Mat3 TranslationMat(const struct Vec2& V);
	static Mat3 ScaleMat(const struct Vec3 V);
	static Mat3 RotationMat(const struct Vec3& V, const float Degrees);
};

struct Mat4
{
	std::array<std::array<float, 4>, 4> values = { {
													{ 0.f, 0.f, 0.f },
													{ 0.f, 0.f, 0.f },
													{ 0.f, 0.f, 0.f }
													}
	};
	Mat4();
	Mat4(std::array<std::array<float, 4>, 4> newValues);
	Mat4(const struct Mat2& newMat);
	Mat4(const struct Mat3& newMat);
	Mat4(const Mat4& newMat);

	std::array<float, 4>& operator[](int row);
	std::array<float, 4> operator[](int row) const;

	friend const Mat4 operator+(const Mat4& M1, const Mat4& M2);
	friend const Mat4 operator-(const Mat4& M1, const Mat4& M2);
	friend const Mat4 operator*(const Mat4& M1, const Mat4& M2);
	Mat4& operator+=(const Mat4& M);
	Mat4& operator-=(const Mat4& M);
	Mat4& operator*=(const Mat4& M);
	friend const Mat4 operator*(const Mat4& M1, const float& F);
	friend const Mat4 operator*(const float& F, const Mat4& M1);
	friend const Mat4& operator*=(Mat4& M1, const float& F);
	friend const struct Vec4 operator*(const Mat4& M1, const struct Vec4& V);
	friend const bool operator==(const Mat4& M1, const Mat4& M2);
	friend const bool operator!=(const Mat4& M1, const Mat4& M2);
	std::string ToString() const;
	friend std::ostream& operator<<(std::ostream& os, const Mat4& M);
	friend std::istream& operator>>(std::istream& is, Mat4& M);

	void Transpose();
	friend const Mat4 Transposed(const Mat4& M);
	float Determinant() const;

	float* GetData() const;

	static Mat4 IdentityMat();
	static Mat4 TranslationMat(const struct Vec3& V);
	static Mat4 ScaleMat(const struct Vec3 V);
	static Mat4 RotationMat(struct Vec4 V, const float Degrees);
	static Mat4 Model(const Vec3 e);
	static Mat4 View(const Vec3 center, const Vec3 eye, const Vec3 up);
	static Mat4 Orthographic(float n, float f, float b, float t, float r, float l);
	static Mat4 Projection(float fov, float aspect, float n, float f);
};