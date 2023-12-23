#include <type_traits>
#include <compare>
#include <cmath>

template <typename T>
    requires std::is_integral_v<T>
class Fraction;

/**
 * @brief Converts a floating-point value to a Fraction with integral numerator and denominator.
 * @tparam Arg_t The type of the input value.
 * @tparam Integral_t The type of the resulting Fraction's numerator and denominator.
 * @param xValue The floating-point value to convert.
 * @param xTolerance The tolerance for considering xValue as zero.
 * @return The Fraction representation of the input value.
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

    Arg_t sign = (xValue < 0) ? -1.0 : 1.0;
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

        x = 1.0 / (x - a);

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
template <typename T>
    requires std::is_integral_v<T>
class Fraction
{
    T mNumerator; ///< Numerator of the fraction.
    T mDenominator; ///< Denominator of the fraction.

    /**
     * @brief Calculates the greatest common divisor (GCD) of two numbers.
     * @param a The first number.
     * @param b The second number.
     * @return The GCD of a and b.
     */
    constexpr T GDC(const T &a, const T &b) noexcept
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
    constexpr T LCM(const T &a, const T &b) noexcept
    {
        if (a == 0 || b == 0)
        {
            return 0;
        }
        return std::abs(a * b) / GDC(a, b);
    }

public:
    constexpr Fraction() noexcept(std::is_nothrow_default_constructible_v<T>)
        requires std::is_default_constructible_v<T>
    = default;

    constexpr explicit Fraction(T &&xNumerator, T &&mDenominator = 1) noexcept(false)
        : mNumerator{xNumerator}, mDenominator{mDenominator}
    {
        if (mDenominator == 0)
            throw std::invalid_argument("Denominator must be unequal zero!");
    }

    constexpr explicit Fraction(const T &xNumerator, const T &mDenominator = 1) noexcept(false)
        : mNumerator{xNumerator}, mDenominator{mDenominator}
    {
        if (mDenominator == 0)
            throw std::invalid_argument("Denominator must be unequal zero!");
    }

    constexpr Fraction(const Fraction &) noexcept(std::is_nothrow_copy_constructible_v<T>)
        requires std::is_copy_constructible_v<T>
    = default;

    constexpr Fraction(Fraction &&) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires std::is_move_constructible_v<T>
    = default;

    constexpr Fraction &operator=(const Fraction &) noexcept(std::is_nothrow_copy_assignable_v<T>)
        requires std::is_copy_assignable_v<T>
    = default;

    constexpr Fraction &operator=(Fraction &&) noexcept(std::is_nothrow_move_assignable_v<T>)
        requires std::is_move_assignable_v<T>
    = default;

    // TODO: Math?
    [[nodiscard]] constexpr auto operator<=>(const Fraction &xIn) const noexcept
    {
        return (mNumerator / mDenominator) <=> (xIn.mNumerator / xIn.mDenominator);
    }

    [[nodiscard]] constexpr bool operator==(const Fraction &xIn) const noexcept
    {
        return mDenominator == xIn.mDenominator && mNumerator == xIn.mNumerator;
    }

    [[nodiscard]] constexpr const T &getDenominator() const noexcept
    {
        return mDenominator;
    }

    [[nodiscard]] constexpr T getDenominator() noexcept
    {
        return mDenominator;
    }

    [[nodiscard]] constexpr const T &getNumerator() const noexcept
    {
        return mNumerator;
    }

    [[nodiscard]] constexpr T getNumerator() noexcept
    {
        return mNumerator;
    }

    [[nodiscard]] constexpr double to_double() const noexcept
    {
        return static_cast<double>(mNumerator) / static_cast<double>(mDenominator);
    }
    [[nodiscard]] constexpr double to_float() const noexcept
    {
        return static_cast<float>(mNumerator) / static_cast<float>(mDenominator);
    }
    [[nodiscard]] constexpr double to_long_double() const noexcept
    {
        return static_cast<long double>(mNumerator) / static_cast<long double>(mDenominator);
    }

    [[nodiscard]] constexpr T GCD() const noexcept
    {
        return GDC(mNumerator, mDenominator);
    }

    [[nodiscard]] constexpr T LCM(const Fraction<T> &other) const noexcept
    {
        return LCM(mDenominator, other.mDenominator);
    }

    constexpr Fraction &simplify() noexcept
    {
        const auto tGDC = GDC(mNumerator, mDenominator);
        mNumerator /= tGDC;
        mDenominator /= tGDC;

        return *this;
    }

    constexpr Fraction<T> &operator+=(const Fraction<T> &xOther)
    {
        if (mNumerator != xOther.mNumerator)
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
    constexpr Fraction<T> &operator+=(const Arg_t &xOther)
        requires std::is_floating_point_v<Arg_t>
    {
        const auto tOther = to_Fraction<Arg_t, T>(xOther);

        return *this += tOther;
    }

    constexpr Fraction<T> &operator+=(const T &xOther)
    {
        return *this += Fraction<T>(xOther, 1);
    }

    constexpr Fraction<T> &operator-=(const Fraction<T> &xOther)
    {
        if (mNumerator != xOther.mNumerator)
        {
            const auto tLCM = LCM(mDenominator, xOther.mDenominator);

            mNumerator = (mNumerator * (tLCM / mDenominator)) - (xOther.mNumerator * (tLCM / xOther.mDenominator));
            mDenominator = tLCM;
        }
        else
        {
            mNumerator += xOther.mNumerator;
        }

        return *this;
    }

    template <typename Arg_t>
    constexpr Fraction<T> &operator-=(const Arg_t &xOther)
        requires std::is_floating_point_v<Arg_t>
    {
        const auto tOther = to_Fraction<Arg_t, T>(xOther);

        return *this -= tOther;
    }

    constexpr Fraction<T> &operator-=(const T &xOther)
    {
        return *this -= Fraction<T>(xOther, 1);
    }

    constexpr Fraction<T> &operator*=(const Fraction<T> &xOther)
    {
        mNumerator *= xOther.mNumerator;
        mDenominator *= xOther.mDenominator;

        return *this;
    }

    template <typename Arg_t>
    constexpr Fraction<T> &operator*=(const Arg_t &xOther)
        requires std::is_floating_point_v<Arg_t>
    {
        const auto tOther = to_Fraction<Arg_t, T>(xOther);

        return *this *= tOther;
    }

    constexpr Fraction<T> &operator*=(const T &xOther)
    {
        mNumerator *= xOther;

        return *this;
    }

    constexpr Fraction<T> &operator/=(const Fraction<T> &xOther)
    {
        mNumerator *= xOther.mDenominator;
        mDenominator *= xOther.mNumerator;

        return *this;
    }

    template <typename Arg_t>
    constexpr Fraction<T> &operator/=(const Arg_t &xOther)
        requires std::is_floating_point_v<Arg_t>
    {
        const auto tOther = to_Fraction<Arg_t, T>(xOther);

        return *this /= tOther;
    }

    constexpr Fraction<T> &operator/=(const T &xOther)
    {
        mDenominator *= xOther;

        return *this;
    }

    constexpr friend Fraction<T> operator+(Fraction<T> lhs, const Fraction<T> &rhs)
    {
        lhs += rhs;
        return lhs;
    }

    constexpr friend Fraction<T> operator-(Fraction<T> lhs, const Fraction<T> &rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    constexpr friend Fraction<T> operator*(Fraction<T> lhs, const Fraction<T> &rhs)
    {
        lhs *= rhs;
        return lhs;
    }

    constexpr friend Fraction<T> operator/(Fraction<T> lhs, const Fraction<T> &rhs)
    {
        lhs /= rhs;
        return lhs;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<T> operator+(Fraction<T> lhs, const Arg_t &rhs)
    {
        lhs += rhs;
        return lhs;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<T> operator-(Fraction<T> lhs, const Arg_t &rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<T> operator*(Fraction<T> lhs, const Arg_t &rhs)
    {
        lhs *= rhs;
        return lhs;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<T> operator/(Fraction<T> lhs, const Arg_t &rhs)
    {
        lhs /= rhs;
        return lhs;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<T> operator+(const Arg_t &lhs, Fraction<T> rhs)
    {
        rhs += lhs;
        return rhs;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<T> operator-(const Arg_t &lhs, const Fraction<T> &rhs)
    {
        auto tTemp = to_Fraction<Arg_t, T>(lhs);
        tTemp -= rhs;
        return tTemp;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<T> operator*(const Arg_t &lhs, Fraction<T> rhs)
    {
        rhs *= lhs;
        return rhs;
    }

    template <typename Arg_t>
        requires std::is_floating_point_v<Arg_t>
    constexpr friend Fraction<T> operator/(const Arg_t &lhs, Fraction<T> rhs)
    {
        auto tTemp = to_Fraction<Arg_t, T>(lhs);
        tTemp /= rhs;
        return tTemp;
    }
};
