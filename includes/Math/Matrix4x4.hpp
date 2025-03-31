/*
* Created by Sarthak Rai on 3/31/25.
*/

#pragma once

#include <vector>
#include <array>    // Stack storage
#include <immintrin.h>  // SSE/AVX intrinsics

namespace Math {
    class Matrix {
    public:
        // Init with dimensions
        Matrix(size_t rows, size_t cols);

        // Init from 2D vector
        explicit Matrix(std::vector<std::vector<double>>&& data);

        // Copy Constructor
        Matrix(const Matrix& other);

        // Move Constructor
        Matrix(Matrix&& other) noexcept;

        ~Matrix() = default;

        [[nodiscard]] size_t rows() const { return rows_; }
        [[nodiscard]] size_t cols() const { return cols_; }

        double& operator()(size_t row, size_t col);
        const double& operator()(size_t row, size_t col) const;

        // Copy & Move
        Matrix& operator=(const Matrix& other);
        Matrix& operator=(Matrix&& other) noexcept;

        // Addition, Multiplication & Subtraction
        Matrix operator+(const Matrix& other) const;
        Matrix operator-(const Matrix& other) const;
        Matrix operator*(const Matrix& other) const;

        Matrix operator*(double scalar) const;

        [[nodiscard]] Matrix transpose() const;
    private:
        // Ints
        size_t rows_;
        size_t cols_;
        double init_value_ = 0.0;

        // Vectors
        std::vector<std::vector<double>> data_;
    };

    Matrix operator*(double scalar, const Matrix& matrix);
}

