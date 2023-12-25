#include <type_traits>
#include <compare>
#include <cmath>
#include <stdexcept>

template <typename Type>
    requires std::is_integral_v<Type>
class Fraction;

/**
 * @brief Converts a floating-point value to a Fraction with integral numerator and denominator.
 * @tparam Arg_t The type of the input value.
 * @tparam Integral_t The type of the resulting Fraction's numerator and denominator.
 * @param xValue The floating-point value to convert.
 * @param xTolerance The tolerance for considering xValue as zero.
 * @return The Fraction representation of the input value.
 * @exception std::invalid_argument if xTolerance is less than or equal to zero.
 */
template <typename Arg_t, typename Integral_t = int>
[[nodiscard]] constexpr Fraction<Integral_t> to_Fraction(Arg_t xValue, Arg_t xTolerance = std::numeric_limits<Arg_t>::epsilon()) noexcept(false)
    requires std::is_floating_point_v<Arg_t> && std::is_integral_v<Integral_t>
{
    if (xTolerance <= 0)
        throw std::invalid_argument("Tolerance must be greater than zero.");

    if (xValue < xTolerance && xValue > -xTolerance)
    {
        return Fraction<Integral_t>{0, 1};
    }

    Arg_t sign = (xValue < 0) ? static_cast<Arg_t>(-1.0) : static_cast<Arg_t>(1.0);
    xValue = std::abs(xValue);

    Arg_t epsilon = xTolerance;
    Arg_t h1 = 1, h2 = 0, k1 = 0, k2 = 1;
    Arg_t x = xValue;

    while (true)
    {
        Arg_t a = std::floor(x);
        Arg_t tmp = h1;
        h1 = a * h1 + h2;
        h2 = tmp;
        tmp = k1;
        k1 = a * k1 + k2;
        k2 = tmp;

        x = static_cast<Arg_t>(1.0) / (x - a);

        if (std::abs(h1 / k1 - xValue) < xValue * epsilon)
        {
            return Fraction<Integral_t>{static_cast<Integral_t>(sign * h1), static_cast<Integral_t>(k1)};
        }
    }

    return Fraction<Integral_t>{0, 0};
}

/**
 * @brief Represents a fraction with integral numerator and denominator.
 */
template <typename Type>
    requires std::is_integral_v<Type>
class Fraction
{
    Type mNumerator{0}; ///< Numerator of the fraction.
    Type mDenominator{1}; ///< Denominator of the fraction.

    /**
     * @brief Calculates the greatest common divisor (GCD) of two numbers.
     * @param a The first number.
     * @param b The second number.
     * @return The GCD of a and b.
     */
    constexpr Type GDC(const Type &a, const Type &b) noexcept
    {
        auto tResult = std::min(a, b);
        while (tResult > 0)
        {
            if (a % tResult == 0 && b % tResult == 0)
            {
                break;
            }
            --tResult;
        }

        return tResult;
    }

    /**
     * @brief Calculates the least common multiple (LCM) of two numbers.
     * @param a The first number.
     * @param b The second number.
     * @return The LCM of a and b.
     */
    constexpr Type LCM(const Type &a, const Type &b) noexcept
    {
        if (a == 0 || b == 0)
        {
            return 0;
        }
        return std::abs(a * b) / GDC(a, b);
    }

public:
    /**
     * Default constructor.
     * Constructs a Fraction with a numerator of 0 and denominator of 1.
     */
    constexpr Fraction() noexcept(std::is_nothrow_default_constructible_v<Type>)
        requires std::is_default_constructible_v<Type>
    = default;

    /**
     * @brief Constructs a Fraction from a numerator and optional denominator.
     *
     * This is an explicit constructor that allows constructing a Fraction from a
     * numerator and optional denominator. If the denominator is not provided, it
     * defaults to 1. The constructor will throw an invalid_argument exception if
     * the denominator is 0.
     *
     * @param xNumerator - The numerator value for the fraction.
     * @param mDenominator - Optional denominator value, defaults to 1 if not provided.
     * @exception std::invalid_argument - If the denominator is 0.
     */
    constexpr explicit Fraction(Type &&xNumerator, Type &&mDenominator = 1) noexcept(false)
        : mNumerator{xNumerator}, mDenominator{mDenominator}
    {
        if (mDenominator == 0)
            throw std::invalid_argument("Denominator must be unequal zero!");
    }

    /**
     * @brief Constructs a Fraction from a numerator and denominator.
     *
     * This constructor allows creating a Fraction from a numerator and denominator.
     * The denominator defaults to 1 if not provided.
     * Throws an invalid_argument exception if the denominator is 0.
     *
     * @param xNumerator - The numerator value for the fraction.
     * @param mDenominator - The denominator value for the fraction. Defaults to 1.
     * @exception std::invalid_argument - If the denominator is 0.
     */
    constexpr explicit Fraction(const Type &xNumerator, const Type &mDenominator = 1) noexcept(false)
        : mNumerator{xNumerator}, mDenominator{mDenominator}
    {
        if (mDenominator == 0)
            throw std::invalid_argument("Denominator must be unequal zero!");
    }

    /**
     * @brief Copy constructor.
     * Constructs a Fraction from another Fraction, copying its numerator and denominator.
     * This constructor is declared as defaulted to use the compiler-generated version.
     */
    constexpr Fraction(const Fraction &) noexcept(std::is_nothrow_copy_constructible_v<Type>)
        requires std::is_copy_constructible_v<Type>
    = default;

    /**
     * @brief Move constructor.
     * Constructs a Fraction by moving from another Fraction.
     */
    constexpr Fraction(Fraction &&) noexcept(std::is_nothrow_move_constructible_v<Type>)
        requires std::is_move_constructible_v<Type>
    = default;

    /**
     * @brief Copy assignment operator.
     * Assigns the value of another Fraction to this Fraction,
     * replacing the current value.
     * Uses the default implementation.
     */
    constexpr Fraction &operator=(const Fraction &) noexcept(std::is_nothrow_copy_assignable_v<Type>)
        requires std::is_copy_assignable_v<Type>
    = default;

    /**
     * @brief Move assignment operator.
     * Assigns the value of another Fraction to this Fraction
     * by moving from that Fraction.
     */
    constexpr Fraction &operator=(Fraction &&) noexcept(std::is_nothrow_move_assignable_v<Type>)
        requires std::is_move_assignable_v<Type>
    = default;

    /**
     * @brief Compares this Fraction with another Fraction using the spaceship operator.
     * 
     * Returns std::strong_ordering::less if this Fraction is less than the other,
     * std::strong_ordering::greater if this Fraction is greater than the other,
     * and std::strong_ordering::equal if they are equal.
     * Fractions with a denominator of 0 are considered equal.
     */
    [[nodiscard]] constexpr auto operator<=>(const Fraction &xIn) const noexcept
    {
        if (mDenominator == 0 || xIn.mDenominator == 0)
        {
            return std::strong_ordering::equal;
        }

        auto lhsRatio = static_cast<long double>(mNumerator) / mDenominator;
        auto rhsRatio = static_cast<long double>(xIn.mNumerator) / xIn.mDenominator;

        if (lhsRatio < rhsRatio)
            return std::strong_ordering::less;
        if (lhsRatio > rhsRatio)
            return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    /**
     * @brief Compares this Fraction with another Fraction for equality using the equality operator.
     *
     * Returns true if this Fraction and the other Fraction have equal numerators and denominators.
     * Returns false otherwise.
     */
    [[nodiscard]] constexpr bool operator==(const Fraction &xIn) const noexcept
    {
        return mDenominator == xIn.mDenominator && mNumerator == xIn.mNumerator;
    }

    /**
     * @brief Gets a constant reference to the denominator of this fraction.
     *
     * @return A constant reference to the denominator.
     */
    [[nodiscard]] constexpr const Type &getDenominator() const noexcept
    {
        return mDenominator;
    }

    /**
     * @brief Gets the denominator for this fraction.
     *
     * @return The denominator.
     */
    [[nodiscard]] constexpr Type getDenominator() noexcept
    {
        return mDenominator;
    }

    /**
     * @brief Gets a constant reference to the numerator of this fraction.
     *
     * @return A constant reference to the numerator.
     */
    [[nodiscard]] constexpr const Type &getNumerator() const noexcept
    {
        return mNumerator;
    }

    /**
     * @brief Gets the numerator for this fraction.
     *
     * @return The numerator.
     */
    [[nodiscard]] constexpr Type getNumerator() noexcept
    {
        return mNumerator;
    }

    /**
     * @brief Converts the fraction to a double precision floating point number.
     *
     * @return The fraction converted to a double.
     */
    [[nodiscard]] constexpr double to_double() const noexcept
    {
        return static_cast<double>(mNumerator) / static_cast<double>(mDenominator);
    }
    /**
     * @brief Converts the fraction to a single precision floating point number.
     *
     * @return The fraction converted to a float.
     */
    [[nodiscard]] constexpr double to_float() const noexcept
    {
        return static_cast<float>(mNumerator) / static_cast<float>(mDenominator);
    }
    /**
     * @brief Converts the fraction to a long double precision floating point number.
     *
     * @return The fraction converted to a long double.
     */
    [[nodiscard]] constexpr double to_long_double() const noexcept
    {
        return static_cast<long double>(mNumerator) / static_cast<long double>(mDenominator);
    }

    /**
     * @brief Returns the greatest common divisor of the numerator and denominator.
     */
    [[nodiscard]] constexpr Type GCD() const noexcept
    {
        return GDC(mNumerator, mDenominator);
    }

    /**
     * Calculates the least common multiple of this fraction's denominator
     * and the passed fraction's denominator.
     *
     * @param other The other fraction to calculate the LCM with.
     * @return The least common multiple of the denominators.
     */
    [[nodiscard]] constexpr Type LCM(const Fraction<Type> &other) const noexcept
    {
        return LCM(mDenominator, other.mDenominator);
    }

    /** 
     * @brief Simplifies the fraction by dividing both the numerator and denominator by their greatest common divisor.
     * @return Returns a reference to the simplified fraction.
     **/
    constexpr Fraction &simplify() noexcept
    {
        const auto tGDC = GDC(mNumerator, mDenominator);
        mNumerator /= tGDC;
        mDenominator /= tGDC;

        return *this;
    }

    constexpr Fraction<Type> &operator+=(const Fraction<Type> &xOther)
    {
        if (mDenominator != xOther.mDenominator)
        {
            const auto tLCM = LCM(mDenominator, xOther.mDenominator);

            mNumerator = (mNumerator * (tLCM / mDenominator)) + (xOther.mNumerator * (tLCM / xOther.mDenominator));
            mDenominator = tLCM;
        }
        else
        {
            mNumerator += xOther.mNumerator;
        }

        return *this;
    }

    template <typename Arg_t>
    constexpr Fraction<Type> &operator+=(const Arg_t &xOther)
        requires std::is_floating_point_v<Arg_t>
    {
        const auto tOther = to_Fraction<Arg_t, Type>(xOther);

        return *this += tOther;
    }

    constexpr Fraction<Type> &operator+=(const Type &xOther)
    {
        return *this += Fraction<Type>(xOther, 1);
    }

    constexpr Fraction<Type> &operator-=(const Fraction<Type> &xOther)
    {
        if (mDenominator != xOther.mDenominator)
        {
            const auto tLCM = LCM(mDenominator, xOther.mDenominator);

            mNumerator = (mNumerator * (tLCM / mDenominator)) - (xOther.mNumerator * (tLCM / xOther.mDenominator));
            mDenominator = tLCM;
        }
        else
        {
            mNumerator -= xOther.mNumerator;
        }

        return *this;
    }

    template <typename Arg_t>
    constexpr Fraction<Type> &operator-=(const Arg_t &xOther)
        requires std::is_floating_point_v<Arg_t>
    {
        const auto tOther = to_Fraction<Arg_t, Type>(xOther);

        return *this -= tOther;
    }

    constexpr Fraction<Type> &operator-=(const Type &xOther)
    {
        return *this -= Fraction<Type>(xOther, 1);
    }

    constexpr Fraction<Type> &operator*=(const Fraction<Type> &xOther)
    {
        mNumerator *= xOther.mNumerator;
        mDenominator *= xOther.mDenominator;

        return *this;
    }

    template <typename Arg_t>
    constexpr Fraction<Type> &operator*=(const Arg_t &xOther)
        requires std::is_floating_point_v<Arg_t>
    {
        const auto tOther = to_Fraction<Arg_t, Type>(xOther);

        return *this *= tOther;
    }

    constexpr Fraction<Type> &operator*=(const Type &xOther)
    {
        mNumerator *= xOther;

        return *this;
    }

    constexpr Fraction<Type> &operator/=(const Fraction<Type> &xOther)
    {
        mNumerator *= xOther.mDenominator;
        mDenominator *= xOther.mNumerator;

        return *this;
    }

    template <typename Arg_t>
    constexpr Fraction<Type> &operator/=(const Arg_t &xOther)
        requires std::is_floating_point_v<Arg_t>
    {
        const auto tOther = to_Fraction<Arg_t, Type>(xOther);

        return *this /= tOther;
    }

    constexpr Fraction<Type> &operator/=(const Type &xOther)
    {
        mDenominator *= xOther;

        return *this;
    }

    constexpr friend Fraction<Type> operator+(Fraction<Type> lhs, const Fraction<Type> &rhs)
    {
        lhs += rhs;
        return lhs;
    }

    constexpr friend Fraction<Type> operator-(Fraction<Type> lhs, const Fraction<Type> &rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    constexpr friend Fraction<Type> operator*(Fraction<Type> lhs, const Fraction<Type> &rhs)
    {
        lhs *= rhs;
        return lhs;
    }

    constexpr friend Fraction<Type> operator/(Fraction<Type> lhs, const Fraction<Type> &rhs)
    {
        lhs /= rhs;
        return lhs;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<Type> operator+(Fraction<Type> lhs, const Arg_t &rhs)
    {
        lhs += rhs;
        return lhs;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<Type> operator-(Fraction<Type> lhs, const Arg_t &rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<Type> operator*(Fraction<Type> lhs, const Arg_t &rhs)
    {
        lhs *= rhs;
        return lhs;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<Type> operator/(Fraction<Type> lhs, const Arg_t &rhs)
    {
        lhs /= rhs;
        return lhs;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<Type> operator+(const Arg_t &lhs, Fraction<Type> rhs)
    {
        rhs += lhs;
        return rhs;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<Type> operator-(const Arg_t &lhs, const Fraction<Type> &rhs)
    {
        auto tTemp = to_Fraction<Arg_t, Type>(lhs);
        tTemp -= rhs;
        return tTemp;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<Type> operator*(const Arg_t &lhs, Fraction<Type> rhs)
    {
        rhs *= lhs;
        return rhs;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<Type> operator/(const Arg_t &lhs, Fraction<Type> rhs)
    {
        auto tTemp = to_Fraction<Arg_t, Type>(lhs);
        tTemp /= rhs;
        return tTemp;
    }
};
