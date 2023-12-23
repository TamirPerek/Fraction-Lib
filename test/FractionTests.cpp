#include "../src/Fraction.h"

#include <gtest/gtest.h>
#include <random>

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

    EXPECT_THROW(Fraction(1,0), std::invalid_argument);
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
