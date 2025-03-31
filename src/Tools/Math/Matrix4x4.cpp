/*
* Created by Sarthak Rai on 3/31/25.
*/

#include "Math/Matrix.hpp"
#include <ostream>
#include <utility>

namespace Math
{
    // Init with dimensions
    Matrix::Matrix(const size_t rows, const size_t cols) : rows_(rows), cols_(cols) {
        if (rows_ <= 0 || cols_ <= 0) {
            throw std::invalid_argument("Matrix dimensions must be positive");
        }
    }

    // Init from 2D vector
    Matrix::Matrix(std::vector<std::vector<double>>&& data)
    : rows_(data.size()), cols_(data.empty() ? 0 : data[0].size()), data_(std::move(data)) {
        for (const auto& row : data) {
            if (row.size() != cols_) {
                throw std::invalid_argument("Matrix dimensions must be rectangular");
            }
        }
    }

    // Copy Constructor
    Matrix::Matrix(const Matrix& other) : rows_(other.rows_), cols_(other.cols_), data_(other.data_) {}

    // Move Constructor
    Matrix::Matrix(Matrix&& other) noexcept : rows_(other.rows_), cols_(other.cols_), data_(std::move(other.data_)) {
        other.rows_ = 0;
        other.cols_ = 0;
    }

    // Copy Assignment
    Matrix& Matrix::operator=(const Matrix& other) {
        if (this != &other) {
            rows_ = other.rows_;
            cols_ = other.cols_;
            data_ = other.data_;
        }

        return *this;
    }

    // Move Assignment
    Matrix& Matrix::operator=(Matrix&& other) noexcept {
        if (this != &other) {
            rows_ = other.rows_;
            cols_ = other.cols_;
            data_ = std::move(other.data_);

            other.rows_ = 0;
            other.cols_ = 0;
        }

        return *this;
    }

    // Access element with bounds checking
    double& Matrix::operator()(size_t row, size_t col) {
        if (row >= rows_ || col >= cols_) {
            throw std::out_of_range("Matrix index out of range");
        }

        return data_[row][col];
    }

    // Const access element with bounds checking
    const double& Matrix::operator()(size_t row, size_t col) const {
        if (row >= rows_ || col >= cols_) {
            throw std::out_of_range("Matrix index out of range");
        }

        return data_[row][col];
    }

    // Addition
    Matrix Matrix::operator+(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Matrix dimensions must match for addition");
        }

        Matrix result(rows_, cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                result(i, j) = data_[i][j] + other(i, j);
            }
        }

        return result;
    }

    // Subtraction
    Matrix Matrix::operator-(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Matrix dimensions must match for subtraction");
        }

        Matrix result(rows_, other.cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < other.cols_; ++j) {
                result(i, j) = data_[i][j] - other(i, j);
            }
        }

        return result;
    }

    // Multiplication
    Matrix Matrix::operator*(const Matrix& other) const {
        if (cols_ != other.rows_) {
            throw std::invalid_argument("Cols of M1 must match for rows of M2 for multiplication");
        }

        Matrix result(rows_, other.cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < other.cols_; ++j) {
                double sum = 0.0;
                for (size_t k = 0; k < cols_; ++k) {
                    sum += data_[i][k] * other(k, j);
                }
                result(i, j) = sum;
            }
        }

        return result;
    }

    // Scalar multiplication
    Matrix Matrix::operator*(double scalar) const {
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                result(i, j) = data_[i][j] * scalar;
            }
        }

        return result;
    }

    // Transpose
    Matrix Matrix::transpose() const {
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                result(j, i) = data_[i][j];
            }
        }

        return result;
    }

    // Non-Member scalar multiplication
    Matrix operator*(double scalar, const Matrix& matrix) { return matrix * scalar; }
}
