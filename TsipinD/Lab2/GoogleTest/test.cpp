#include "pch.h"
#include <stdexcept> 
#include "../utmatrix.h"
// ===== TVector ======

TEST(TVector, vector_length) {
    EXPECT_NO_THROW(TVector<int> v(5));
    EXPECT_THROW(TVector<int> v(-5), std::out_of_range);
    EXPECT_THROW(TVector<int> v(MAX_VECTOR_SIZE + 1), std::out_of_range);
}

TEST(TVector, vector_start_index) {
    EXPECT_NO_THROW(TVector<int> v(5, 3));
    EXPECT_THROW(TVector<int> v(5, -1), std::out_of_range);
}

TEST(TVector, copied_vector_is_equal) {
    TVector<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;
    TVector<int> v2(v);
    EXPECT_EQ(v, v2);
}

TEST(TVector, copied_vector_has_its_own_memory) {
    TVector<int> v(3);
    v[0] = 1;
    TVector<int> v2(v);
    v2[0] = 100;
    EXPECT_NE(v[0], v2[0]);
}

TEST(TVector, getter) {
    TVector<int> v(4, 2);
    EXPECT_EQ(4, v.GetSize());
    EXPECT_EQ(2, v.GetStartIndex());
}

TEST(TVector, vector_index_is_out_of_range) {
    TVector<int> v(3);
    EXPECT_THROW(v[-1], std::out_of_range);
    EXPECT_THROW(v[3], std::out_of_range);
}

TEST(TVector, equality_operator) {
    TVector<int> v1(3), v2(3);
    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[0] = 1; v2[1] = 2; v2[2] = 3;
    EXPECT_TRUE(v1 == v2);
    TVector<int> vec1(3), vec2(3);
    vec1[0] = 1; vec2[0] = 2;
    EXPECT_TRUE(vec1 != vec2);
}

TEST(TVector, assign_vector) {
    TVector<int> v(3);
    v[0] = 1;
    EXPECT_NO_THROW(v = v);
    TVector<int> v1(3), v2(3);
    v1[0] = 5;
    v2 = v1;
    EXPECT_EQ(v1, v2);
    TVector<int> vec1(3), vec2(5);
    v1[0] = 7;
    vec2 = vec1;
    EXPECT_EQ(vec1, vec2);
}

TEST(TVector, vector_and_digit) {
    TVector<int> v(3, 0);
    v[0] = 10; v[1] = 20; v[2] = 30;
    TVector<int> res = v + 5;
    EXPECT_EQ(15, res[0]);
    EXPECT_EQ(25, res[1]);
    EXPECT_EQ(35, res[2]);

    TVector<int> res2 = v - 10;
    EXPECT_EQ(0, res2[0]);
    EXPECT_EQ(10, res2[1]);
    EXPECT_EQ(20, res2[2]);

    TVector<int> res3 = v * 2;
    EXPECT_EQ(20, res3[0]);
    EXPECT_EQ(40, res3[1]);
    EXPECT_EQ(60, res3[2]);
}

TEST(TVector, vectors_plus_minus) {
    TVector<int> v1(3), v2(3);
    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[0] = 4; v2[1] = 5; v2[2] = 6;
    TVector<int> res = v1 + v2;
    EXPECT_EQ(5, res[0]);
    EXPECT_EQ(7, res[1]);
    EXPECT_EQ(9, res[2]);

    TVector<int> res2 = v2 - v1;
    EXPECT_EQ(3, res2[0]);
    EXPECT_EQ(3, res2[1]);
    EXPECT_EQ(3, res2[2]);

    TVector<int> vec1(3), vec2(4);
    EXPECT_THROW(vec1 + vec2, std::invalid_argument);
    EXPECT_THROW(vec1 - vec2, std::invalid_argument);
}

TEST(TVector, multiply_vectors) {
    TVector<int> v1(3), v2(3);
    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[0] = 4; v2[1] = 5; v2[2] = 6;
    int res = v1 * v2;
    EXPECT_EQ(32, res); // 1*4 + 2*5 + 3*6

    TVector<int> vec1(3), vec2(4);
    EXPECT_THROW(vec1 * vec2, std::invalid_argument);
}

// =============== TMatrix Tests =========================

TEST(TMatrix, matrix_length_and_creation) {
    EXPECT_NO_THROW(TMatrix<int> m(3));
    EXPECT_THROW(TMatrix<int> m(-1), std::out_of_range);
    EXPECT_THROW(TMatrix<int> m(MAX_MATRIX_SIZE + 1), std::out_of_range);

    TMatrix<int> m(3);
    EXPECT_EQ(3, m.GetSize());
}

TEST(TMatrix, copy_and_assigned_matrix) {
    TMatrix<int> m1(3);
    m1[0][0] = 5;
    TMatrix<int> m2(m1);
    EXPECT_EQ(m1, m2);

    m2[0][0] = 10;
    EXPECT_NE(m1, m2);

    TMatrix<int> m3(3);
    m3 = m1;
    EXPECT_EQ(m1, m3);

    TMatrix<int> m4(5);
    m4 = m1;
    EXPECT_EQ(m1, m4);
}

TEST(TMatrix, equal_operator) {
    TMatrix<int> m1(2), m2(2);
    m1[0][0] = 1; m2[0][0] = 1;
    EXPECT_TRUE(m1 == m2);

    m2[0][0] = 2;
    EXPECT_TRUE(m1 != m2);
}

TEST(TMatrix, plus_minus_operations) {
    TMatrix<int> m1(2), m2(2);
    m1[0][0] = 1; m2[0][0] = 2;

    TMatrix<int> sum = m1 + m2;
    EXPECT_EQ(3, sum[0][0]);

    TMatrix<int> diff = m2 - m1;
    EXPECT_EQ(1, diff[0][0]);

    TMatrix<int> m3(3);
    EXPECT_THROW(m1 + m3, std::invalid_argument);
    EXPECT_THROW(m1 - m3, std::invalid_argument);
}

TEST(TMatrix, index_test) {
    TMatrix<int> m(3);
    m[0][0] = 42;
    EXPECT_EQ(42, m[0][0]);

    EXPECT_THROW(m[-1], std::out_of_range);
    EXPECT_THROW(m[3], std::out_of_range);
}