#pragma once
#include <beingEngine/core/core.hpp>

template<size_t t_width, size_t t_height>
class Matrix {
	friend class Matrix<t_height, t_width>;

public:
	Matrix();
	template<typename = std::enable_if_t<((t_width + t_height) > 2)>>
	Matrix(const float value);
	template<typename... Args, typename = std::enable_if_t<sizeof...(Args) == (t_width * t_height)>>
	Matrix(const Args... args);

	const float* const getData() const { return (float*)m_data; }
	float* const getData() { return (float*)m_data; }
	inline const size_t getWidth() { return t_width; };
	inline const size_t getHeight() { return t_height; };

	const Matrix<t_height, t_width> transpose() const;

	const Matrix<t_width, t_height> operator+(const Matrix<t_width, t_height>& other) const;
	const Matrix<t_width, t_height> operator+(const float scalar) const;

	const Matrix<t_width, t_height> operator-(const Matrix<t_width, t_height>& other) const;
	const Matrix<t_width, t_height> operator-(const float scalar) const;

	template<size_t o_width>
	const Matrix<o_width, t_height> operator*(const Matrix<o_width, t_width>& other) const;
	const Matrix<t_width, t_height> operator*(const float scalar) const;

	const float* operator[](const size_t index) const;
	float* operator[](const size_t index);

	const bool operator==(const Matrix<t_width, t_height>& other) const;

private:
	float m_data[t_width][t_height];
};

#include "matrix.ipp"