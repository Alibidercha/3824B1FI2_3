#include "pch.h"
#include <stdexcept>
#include "../Lab3/TStack.h"
#include "../Lab3/TPostfix.h"
#include "../Lab3/TPostfix.cpp"

// =================== TStack Tests ==============================

TEST(TStack, can_create_stack_with_positive_size) {
    EXPECT_NO_THROW(TStack<int> st(10));
}

TEST(TStack, cannot_create_stack_with_negative_size) {
    EXPECT_THROW(TStack<int> st(-5), std::out_of_range);
}

TEST(TStack, new_stack_is_empty) {
    TStack<int> st(5);
    EXPECT_TRUE(st.IsEmpty());
}

TEST(TStack, push_makes_stack_not_empty) {
    TStack<int> st(5);
    st.Push(10);
    EXPECT_FALSE(st.IsEmpty());
}

TEST(TStack, pop_from_empty_throws) {
    TStack<int> st(5);
    EXPECT_THROW(st.Pop(), std::runtime_error);
}

TEST(TStack, top_from_empty_throws) {
    TStack<int> st(5);
    EXPECT_THROW(st.Top(), std::runtime_error);
}

TEST(TStack, push_and_pop_work_correctly) {
    TStack<int> st(5);
    st.Push(1);
    st.Push(2);
    st.Push(3);
    EXPECT_EQ(st.Pop(), 3);
    EXPECT_EQ(st.Pop(), 2);
    EXPECT_EQ(st.Pop(), 1);

    TStack<std::string> st2(3);
    st2.Push("abc");
    EXPECT_EQ(st2.Pop(), "abc");
}

TEST(TStack, top_works_correctly) {
    TStack<int> st(5);
    st.Push(42);
    EXPECT_EQ(st.Top(), 42);
    EXPECT_FALSE(st.IsEmpty());

    TStack<double> st2(3);
    st2.Push(2.5);
    EXPECT_DOUBLE_EQ(st2.Top(), 2.5);
}

TEST(TStack, stack_overflow_throws) {
    TStack<int> st(2);
    st.Push(1);
    st.Push(2);
    EXPECT_THROW(st.Push(3), std::runtime_error);
}

TEST(TStack, stack_underflow_throws) {
    TStack<int> st(3);
    EXPECT_THROW(st.Pop(), std::runtime_error);
}

TEST(TStack, can_use_stack_with_different_types) {
    TStack<double> st1(3);
    TStack<std::string> st2(3);

    EXPECT_NO_THROW(st1.Push(3.14));
    EXPECT_NO_THROW(st2.Push("hello"));
}

TEST(TStack, copied_stack_is_equal) {
    TStack<int> st1(5);
    st1.Push(10);
    st1.Push(20);

    TStack<int> st2(st1);

    EXPECT_EQ(st2.Pop(), 20);
    EXPECT_EQ(st2.Pop(), 10);
}

TEST(TStack, copied_stack_has_its_own_memory) {
    TStack<int> st1(5);
    st1.Push(10);

    TStack<int> st2(st1);
    st2.Push(20);

    EXPECT_EQ(st1.Pop(), 10);
    EXPECT_EQ(st2.Pop(), 20);
}

TEST(TStack, assigned_stack_is_equal) {
    TStack<int> st1(5);
    st1.Push(1);
    st1.Push(2);

    TStack<int> st2(5);
    st2 = st1;

    EXPECT_EQ(st2.Pop(), 2);
    EXPECT_EQ(st2.Pop(), 1);
}

TEST(TStack, assigned_stack_has_its_own_memory) {
    TStack<int> st1(5);
    st1.Push(100);

    TStack<int> st2(5);
    st2 = st1;

    st2.Push(200);

    EXPECT_EQ(st1.Pop(), 100);
    EXPECT_EQ(st2.Pop(), 200);
}

TEST(TStack, isfull_works_correctly) {
    TStack<int> st(2);
    EXPECT_FALSE(st.IsFull());
    st.Push(1);
    EXPECT_FALSE(st.IsFull());
    st.Push(2);
    EXPECT_TRUE(st.IsFull());
}

// ===================== TPostfix - CheckExpression Tests ==========================

TEST(TPostfix_Syntax, valid_simple_expression) {
    TPostfix pf("1+2*3");
    EXPECT_NO_THROW(pf.ToPostfix());
}

TEST(TPostfix_Syntax, invalid_double_operator) {
    TPostfix pf("1++2");
    EXPECT_THROW(pf.ToPostfix(), std::runtime_error);
}

TEST(TPostfix_Syntax, invalid_brackets_order) {
    TPostfix pf(")(1+2)");
    EXPECT_THROW(pf.ToPostfix(), std::runtime_error);
}

TEST(TPostfix_Syntax, missing_closing_bracket) {
    TPostfix pf("(1+2");
    EXPECT_THROW(pf.ToPostfix(), std::runtime_error);
}

TEST(TPostfix_Syntax, empty_brackets) {
    TPostfix pf("()");
    EXPECT_THROW(pf.ToPostfix(), std::runtime_error);
}

TEST(TPostfix_Syntax, invalid_number_two_points) {
    TPostfix pf("1..2");
    EXPECT_THROW(pf.ToPostfix(), std::runtime_error);
}

TEST(TPostfix_Syntax, number_can_end_with_point) {
    TPostfix pf("1.+2.");
    EXPECT_NO_THROW(pf.ToPostfix());
}

TEST(TPostfix_Syntax, number_cannot_start_with_point) {
    TPostfix pf(".5+1");
    EXPECT_THROW(pf.ToPostfix(), std::runtime_error);
}

TEST(TPostfix_Syntax, variable_can_start_with_underscore) {
    TPostfix pf("_x + 1");
    EXPECT_NO_THROW(pf.ToPostfix());
}

TEST(TPostfix_Syntax, function_ln_does_not_require_brackets) {
    TPostfix pf("ln 5");
    EXPECT_NO_THROW(pf.ToPostfix());
}

TEST(TPostfix_Syntax, unary_minus_is_allowed) {
    TPostfix pf("-1+2");
    EXPECT_NO_THROW(pf.ToPostfix());
}

TEST(TPostfix_Syntax, unary_minus_before_bracket) {
    TPostfix pf("-(1+2)");
    EXPECT_NO_THROW(pf.ToPostfix());
}

// ===================== TPostfix - ToPostfix result tests =======================

TEST(TPostfix_ToPostfix, simple_expression) {
    TPostfix pf("1+2*3");
    pf.ToPostfix();
    EXPECT_EQ("1 2 3 * +", pf.GetPostfix());
}

TEST(TPostfix_ToPostfix, brackets_change_priority) {
    TPostfix pf("(1+2)*3");
    pf.ToPostfix();
    EXPECT_EQ("1 2 + 3 *", pf.GetPostfix());
}

TEST(TPostfix_ToPostfix, unary_minus) {
    TPostfix pf("-5+3");
    pf.ToPostfix();
    EXPECT_EQ("5 ~ 3 +", pf.GetPostfix());
}

TEST(TPostfix_ToPostfix, unary_minus_before_bracket) {
    TPostfix pf("-(1+2)");
    pf.ToPostfix();
    EXPECT_EQ("1 2 + ~", pf.GetPostfix());
}

TEST(TPostfix_ToPostfix, ln_function) {
    TPostfix pf("ln 5");
    pf.ToPostfix();
    EXPECT_EQ("5 ln", pf.GetPostfix());
}

TEST(TPostfix_ToPostfix, ln_function_with_brackets) {
    TPostfix pf("ln(5)");
    pf.ToPostfix();
    EXPECT_EQ("5 ln", pf.GetPostfix());
}

TEST(TPostfix_ToPostfix, variable_expression) {
    TPostfix pf("x*2+y");
    pf.ToPostfix();
    EXPECT_EQ("x 2 * y +", pf.GetPostfix());
}

// ===================== TPostfix Calculate tests ==========================

TEST(TPostfix_Calculate, simple_addition) {
    TPostfix pf("1+2");
    EXPECT_DOUBLE_EQ(3.0, pf.Calculate());
}

TEST(TPostfix_Calculate, priority_multiplication) {
    TPostfix pf("1+2*3");
    EXPECT_DOUBLE_EQ(7.0, pf.Calculate());
}

TEST(TPostfix_Calculate, brackets) {
    TPostfix pf("(1+2)*3");
    EXPECT_DOUBLE_EQ(9.0, pf.Calculate());
}

TEST(TPostfix_Calculate, unary_minus) {
    TPostfix pf("-5+3");
    EXPECT_DOUBLE_EQ(-2.0, pf.Calculate());
}

TEST(TPostfix_Calculate, unary_minus_before_bracket) {
    TPostfix pf("-(1+2)");
    EXPECT_DOUBLE_EQ(-3.0, pf.Calculate());
}

TEST(TPostfix_Calculate, ln_function) {
    TPostfix pf("ln(1)");
    EXPECT_DOUBLE_EQ(0.0, pf.Calculate());
}

TEST(TPostfix_Calculate, division_by_zero_throws) {
    TPostfix pf("1/0");
    EXPECT_THROW(pf.Calculate(), std::runtime_error);
}

TEST(TPostfix_Calculate, complex_expression) {
    TPostfix pf("-(2+3)*ln(1+1)");
    double expected = -(5.0) * std::log(2.0);
    EXPECT_DOUBLE_EQ(expected, pf.Calculate());
}