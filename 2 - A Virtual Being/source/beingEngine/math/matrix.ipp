#pragma once
#include "matrix.hpp"

template<size_t t_width, size_t t_height>
Matrix<t_width, t_height>::Matrix() {
	for (size_t i = 0; i < t_width; i++) {
		for (size_t j = 0; j < t_height; j++) {
			m_data[i][j] = (i == j) ? 1 : 0;
		}
	}
}

template<size_t t_width, size_t t_height>
template<typename>
Matrix<t_width, t_height>::Matrix(const float value) {
	for (size_t i = 0; i < t_width; i++) {
		for (size_t j = 0; j < t_height; j++) {
			m_data[i][j] = value;
		}
	}
}

template<size_t t_width, size_t t_height>
template<typename... Args, typename>
Matrix<t_width, t_height>::Matrix(const Args... args) {
	const float vec[t_width * t_height]{(float)args...};
	for (size_t i = 0; i < t_width; i++) {
		for (size_t j = 0; j < t_height; j++) {
			m_data[i][j] = vec[i + (j * t_width)];
		}
	}
}

template<size_t t_width, size_t t_height>
const Matrix<t_height, t_width> Matrix<t_width, t_height>::transpose() const {
	Matrix<t_height, t_width> result;
	for (size_t i = 0; i < t_width; i++) {
		for (size_t j = 0; j < t_height; j++) {
			result[j][i] = m_data[i][j];
		}
	}
	return result;
}

template<size_t t_width, size_t t_height>
const Matrix<t_width, t_height> Matrix<t_width, t_height>::operator+(
	const Matrix<t_width, t_height>& other) const {
	Matrix<t_width, t_height> result;
	for (size_t i = 0; i < t_width; i++) {
		for (size_t j = 0; j < t_height; j++) {
			result[i][j] = m_data[i][j] + other[i][j];
		}
	}
	return result;
}
template<size_t t_width, size_t t_height>
const Matrix<t_width, t_height> Matrix<t_width, t_height>::operator+(
	const float scalar) const {
	Matrix<t_width, t_height> result;
	for (size_t i = 0; i < t_width; i++) {
		for (size_t j = 0; j < t_height; j++) {
			result[i][j] = m_data[i][j] + scalar;
		}
	}
	return result;
}

template<size_t t_width, size_t t_height>
const Matrix<t_width, t_height> Matrix<t_width, t_height>::operator-(
	const Matrix<t_width, t_height>& other) const {
	Matrix<t_width, t_height> result;
	for (size_t i = 0; i < t_width; i++) {
		for (size_t j = 0; j < t_height; j++) {
			result[i][j] = m_data[i][j] - other[i][j];
		}
	}
	return result;
}
template<size_t t_width, size_t t_height>
const Matrix<t_width, t_height> Matrix<t_width, t_height>::operator-(
	const float scalar) const {
	Matrix<t_width, t_height> result;
	for (size_t i = 0; i < t_width; i++) {
		for (size_t j = 0; j < t_height; j++) {
			result[i][j] = m_data[i][j] - scalar;
		}
	}
	return result;
}

template<size_t t_width, size_t t_height>
template<size_t o_width>
const Matrix<o_width, t_height> Matrix<t_width, t_height>::operator*(
	const Matrix<o_width, t_width>& other) const {
	Matrix<o_width, t_height> result(0);
	for (size_t n = 0; n < o_width; n++) {
		for (size_t l = 0; l < t_height; l++) {
			for (size_t m = 0; m < t_width; m++) {
				result[n][l] += m_data[m][l] * other[n][m];
			}
		}
	}
	return result;
}
template<size_t t_width, size_t t_height>
const Matrix<t_width, t_height> Matrix<t_width, t_height>::operator*(
	const float scalar) const {
	Matrix<t_width, t_height> result;
	for (size_t i = 0; i < t_width; i++) {
		for (size_t j = 0; j < t_height; j++) {
			result[i][j] = m_data[i][j] * scalar;
		}
	}
	return result;
}

template<size_t t_width, size_t t_height>
inline const float* Matrix<t_width, t_height>::operator[](const size_t index) const {
	return m_data[index];
}
template<size_t t_width, size_t t_height>
inline float* Matrix<t_width, t_height>::operator[](const size_t index) {
	return m_data[index];
}

template<size_t t_width, size_t t_height>
const bool Matrix<t_width, t_height>::operator==(const Matrix<t_width, t_height>& other) const {
	return memcmp(
			   m_data,
			   other.m_data,
			   sizeof(m_data)) == 0;
}