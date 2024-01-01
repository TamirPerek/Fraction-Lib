#include "Fraction.h"

#include <gtest/gtest.h>
#include <random>
#include <numbers>

struct FractionTest : public testing::Test
{
};

TEST_F(FractionTest, create)
{
    Fraction<int> tFactorial;
    Fraction tTest{2, 4};
    Fraction tTest2{tTest};
    EXPECT_EQ(tTest2, tTest);
    Fraction tTest3{std::move(tTest)};
    EXPECT_EQ(tTest2, tTest3);
    Fraction<std::size_t> tTest4;

    tFactorial = tTest2;
    tFactorial = std::move(tTest2);

    EXPECT_EQ(tTest3, tFactorial);

    EXPECT_THROW(Fraction(1, 0), std::invalid_argument);
}

TEST_F(FractionTest, toDouble)
{
    EXPECT_DOUBLE_EQ(Fraction(11, 8).to_double(), 1.375);
}
TEST_F(FractionTest, toFloat)
{
    EXPECT_DOUBLE_EQ(Fraction(11, 8).to_float(), static_cast<float>(1.375));
}
TEST_F(FractionTest, toLongDouble)
{
    EXPECT_DOUBLE_EQ(Fraction(11, 8).to_long_double(), static_cast<long double>(1.375));
}

TEST_F(FractionTest, simplify)
{
    Fraction tTest{11534336, 8388608};
    tTest.simplify();

    EXPECT_EQ(tTest, Fraction(11, 8));
    EXPECT_DOUBLE_EQ(tTest.to_double(), 1.375);

    EXPECT_EQ(Fraction(7, 20).simplify(), Fraction(7, 20));

    Fraction<int> f(2, 4);

    f.simplify();

    EXPECT_EQ(f.getNumerator(), 1);
    EXPECT_EQ(f.getDenominator(), 2);
}

TEST_F(FractionTest, CreateFunction)
{
    const auto tTest = to_Fraction(1.375);
    EXPECT_EQ(tTest, Fraction(11, 8));
    EXPECT_EQ(to_Fraction(-1.375), Fraction(-11, 8));

    EXPECT_EQ(to_Fraction(1.375f), Fraction(11, 8));

    {
        const auto tTest2 = to_Fraction<float, int32_t>(1.375f);
        EXPECT_EQ(tTest2, Fraction<int32_t>(11, 8));
    }
    {
        const auto tTest2 = to_Fraction<double, uint32_t>(1.375);
        EXPECT_EQ(tTest2, Fraction<uint32_t>(11, 8));
    }
    {
        const auto tTest2 = to_Fraction<double, int64_t>(1.375);
        EXPECT_EQ(tTest2, Fraction<int64_t>(11, 8));
    }
    {
        const auto tTest2 = to_Fraction<long double, int64_t>(1.375);
        EXPECT_EQ(tTest2, Fraction<int64_t>(11, 8));
    }

    EXPECT_THROW(auto tTemp = to_Fraction(1.375, -1.0), std::invalid_argument);
}

TEST_F(FractionTest, OperatorPlusEquals)
{
    Fraction tTest{3, 4};
    tTest += Fraction{2, 5};

    EXPECT_EQ(tTest, Fraction(23, 20));

    tTest = Fraction{3, 4};
    tTest += 0.4;

    EXPECT_EQ(tTest, Fraction(23, 20));

    tTest = Fraction{3, 4};
    tTest += Fraction{2, 4};

    EXPECT_EQ(tTest, Fraction(5, 4));

    tTest = Fraction{3, 4};
    tTest += 4;

    EXPECT_EQ(tTest, Fraction(19, 4));
}

TEST_F(FractionTest, OperatorMinusEquals)
{
    Fraction tTest{3, 4};
    tTest -= Fraction{2, 5};

    EXPECT_EQ(tTest, Fraction(7, 20));

    tTest = Fraction{3, 4};
    tTest -= 0.4;

    EXPECT_EQ(tTest, Fraction(7, 20));

    tTest = Fraction{3, 4};
    tTest -= Fraction{2, 4};

    EXPECT_EQ(tTest, Fraction(1, 4));

    tTest = Fraction{3, 4};
    tTest -= 4;

    EXPECT_EQ(tTest, Fraction(-13, 4));
}

TEST_F(FractionTest, OperatorMultiplyEquals)
{
    Fraction tTest{3, 4};
    tTest *= Fraction{2, 5};

    EXPECT_EQ(tTest, Fraction(6, 20));
    tTest.simplify();
    EXPECT_EQ(tTest, Fraction(3, 10));

    tTest = Fraction{3, 4};
    tTest *= 0.4;

    EXPECT_EQ(tTest, Fraction(6, 20));
    tTest.simplify();
    EXPECT_EQ(tTest, Fraction(3, 10));

    tTest = Fraction{3, 4};
    tTest *= 4;

    EXPECT_EQ(tTest, Fraction(12, 4));
    tTest.simplify();
    EXPECT_EQ(tTest, Fraction(3, 1));
}

TEST_F(FractionTest, OperatorDivisionEquals)
{
    Fraction tTest{3, 4};
    tTest /= Fraction{2, 5};

    EXPECT_EQ(tTest, Fraction(15, 8));
    tTest.simplify();
    EXPECT_EQ(tTest, Fraction(15, 8));

    tTest = Fraction{3, 4};
    tTest /= 0.4;

    EXPECT_EQ(tTest, Fraction(15, 8));

    tTest = Fraction{3, 4};
    tTest /= 4;

    EXPECT_EQ(tTest, Fraction(3, 16));
    tTest.simplify();
    EXPECT_EQ(tTest, Fraction(3, 16));
}

TEST_F(FractionTest, OperatorPlus)
{
    auto tTest = Fraction{3, 4} + Fraction{2, 5};
    EXPECT_EQ(tTest, Fraction(23, 20));

    tTest = Fraction{3, 4} + 0.4;
    EXPECT_EQ(tTest, Fraction(23, 20));

    tTest = 0.75 + Fraction{2, 5};
    EXPECT_EQ(tTest, Fraction(23, 20));

    tTest = Fraction{3, 4} + Fraction{2, 4};
    EXPECT_EQ(tTest, Fraction(5, 4));
}

TEST_F(FractionTest, OperatorMinus)
{
    auto tTest = Fraction{3, 4} - Fraction{2, 5};
    EXPECT_EQ(tTest, Fraction(7, 20));

    tTest = Fraction{3, 4} - 0.4;
    EXPECT_EQ(tTest, Fraction(7, 20));

    tTest = 0.75 - Fraction{2, 5};
    EXPECT_EQ(tTest, Fraction(7, 20));

    tTest = Fraction{3, 4} - Fraction{2, 4};
    EXPECT_EQ(tTest, Fraction(1, 4));
}

TEST_F(FractionTest, OperatorMultiply)
{
    auto tTest = Fraction{3, 4} * Fraction{2, 5};

    EXPECT_EQ(tTest, Fraction(6, 20));
    tTest.simplify();
    EXPECT_EQ(tTest, Fraction(3, 10));

    tTest = Fraction{3, 4} * 0.4;
    EXPECT_EQ(tTest, Fraction(6, 20));
    tTest.simplify();
    EXPECT_EQ(tTest, Fraction(3, 10));

    tTest = 0.75 * Fraction{2, 5};
    EXPECT_EQ(tTest, Fraction(6, 20));
    tTest.simplify();
    EXPECT_EQ(tTest, Fraction(3, 10));
}

TEST_F(FractionTest, OperatorDivision)
{
    auto tTest = Fraction{3, 4} / Fraction{2, 5};

    EXPECT_EQ(tTest, Fraction(15, 8));
    tTest.simplify();
    EXPECT_EQ(tTest, Fraction(15, 8));

    tTest = Fraction{3, 4} / 0.4;
    EXPECT_EQ(tTest, Fraction(15, 8));

    tTest = 0.75 / Fraction{2, 5};
    EXPECT_EQ(tTest, Fraction(15, 8));
}

TEST_F(FractionTest, ChainOperators)
{
    auto tTest = Fraction{3, 4};
    tTest += Fraction{2, 5} - Fraction{3, 4};

    EXPECT_EQ(tTest, Fraction(8, 20));
    tTest.simplify();
    EXPECT_EQ(tTest, Fraction(2, 5));

    tTest = Fraction{3, 4};
    tTest += 0.4 / Fraction{2, 5} - (Fraction{3, 4} * 0.75);

    EXPECT_EQ(tTest, Fraction(95, 80));
    tTest.simplify();
    EXPECT_EQ(tTest, Fraction(19, 16));
}

// Validating constructor with default values
TEST_F(FractionTest, DefaultConstructor)
{
    Fraction<int> f;

    EXPECT_NE(f.getNumerator(), 0);
    EXPECT_NE(f.getDenominator(), 1);
}

// Testing arithmetic operations
TEST_F(FractionTest, ArithmeticOperations)
{
    Fraction f1(1, 2);
    Fraction f2(1, 3);

    EXPECT_EQ((f1 + f2).getNumerator(), 5);
    EXPECT_EQ((f1 - f2).getNumerator(), 1);

    f1 *= 2;
    EXPECT_EQ(f1, Fraction(2, 2));
    f1.simplify();
    EXPECT_EQ(f1, Fraction(1, 1));

    f1 /= 2;
    EXPECT_EQ(f1, Fraction(1, 2));
}

// Testing comparison operators
TEST_F(FractionTest, ComparisonOperators)
{
    Fraction f1(1, 2);
    Fraction f2(1, 3);

    EXPECT_GT(f1, f2);
    EXPECT_FALSE(f1 == f2);

    EXPECT_LT(Fraction(1, 3), Fraction(1, 2));
    EXPECT_EQ(Fraction(1, 3), Fraction(1, 3));
}

TEST_F(FractionTest, MinusAndPlusOperator)
{
    Fraction f(-2, 1);
    EXPECT_EQ(+f, Fraction(-2, 1));
    EXPECT_EQ(-f, Fraction(2, 1));
}

TEST_F(FractionTest, Swap)
{
    Fraction f1(1, 2);
    Fraction f2(1, 3);

    std::swap(f1, f2);

    EXPECT_EQ(f1, Fraction(1, 3));
    EXPECT_EQ(f2, Fraction(1, 2));
}

TEST_F(FractionTest, Sinus)
{
    auto f1 = to_Fraction(0.5 * std::numbers::pi);
    EXPECT_EQ(::sin(f1), Fraction(1, 1));
}

TEST_F(FractionTest, Cosinus)
{
    auto f1 = to_Fraction(2 * std::numbers::pi);
    EXPECT_EQ(::cos(f1), Fraction(1, 1));
}

TEST_F(FractionTest, Tangens)
{
    auto f1 = to_Fraction(0.25 * std::numbers::pi);
    EXPECT_EQ(::tan(f1), Fraction(1, 1));
}

TEST_F(FractionTest, Pow)
{
    Fraction f1{2, 3};
    EXPECT_EQ(::pow(f1, 2), Fraction(4, 9));
}

TEST_F(FractionTest, Sqrt)
{
    Fraction f1{4, 9};
    EXPECT_EQ(::sqrt(f1), Fraction(2, 3));
}

TEST_F(FractionTest, Atan)
{
    Fraction x{1, 2};
    Fraction expected = to_Fraction(atan(0.5));
    Fraction actual = atan(x);
    EXPECT_EQ(expected, actual);

    x = Fraction{3, 5};
    expected = to_Fraction(atan(0.6));
    actual = atan(x);
    EXPECT_EQ(expected, actual);

    x = Fraction{0, 1};
    expected = Fraction{0, 1};
    actual = atan(x);
    EXPECT_EQ(expected, actual);

    x = Fraction{-1, 2};
    expected = to_Fraction(atan(-0.5));
    actual = atan(x);
    EXPECT_EQ(expected, actual);

    x = Fraction{1234, 5678};
    expected = to_Fraction(atan(1234.0 / 5678));
    actual = atan(x);
    EXPECT_FLOAT_EQ(expected.to_double(), actual.to_double());
}

TEST_F(FractionTest, Hypot)
{
    Fraction x{9, 3};
    Fraction y{16, 4};
    Fraction expected{5};
    Fraction actual = hypot(x, y);

    EXPECT_EQ(expected, actual);

    x = Fraction{0};
    y = Fraction{0};

    expected = Fraction{0};
    actual = hypot(x, y);

    EXPECT_EQ(expected, actual);

    x = Fraction{-9, 3};
    y = Fraction{-16, 4};

    expected = Fraction{5};
    actual = hypot(x, y);

    EXPECT_EQ(expected, actual);

    x = Fraction{-9, 3};
    y = Fraction{16, 4};

    expected = Fraction{5};
    actual = hypot(x, y);

    EXPECT_EQ(expected, actual);
}

TEST_F(FractionTest, Atan2)
{
    Fraction x{1, 2};
    Fraction y{1, 4};
    Fraction expected = to_Fraction(atan2(0.25, 0.5));
    Fraction actual = atan2(y, x);
    EXPECT_FLOAT_EQ(expected.to_double(), actual.to_double());

    x = Fraction{-1, 2};
    y = Fraction{-1, 4};
    expected = to_Fraction(::atan2(-0.25, -0.5));
    actual = ::atan2(y, x);
    EXPECT_FLOAT_EQ(expected.to_double(), actual.to_double());

    x = Fraction{1234, 5678};
    y = Fraction{5678, 1234};
    expected = to_Fraction(atan2(5678.0 / 1234, 1234.0 / 5678));
    actual = atan2(y,x);
    EXPECT_EQ(expected, actual);
}
