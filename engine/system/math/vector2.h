#pragma once

#include <cmath>
#include <ostream>
#include <algorithm>

namespace raid
{
   
template<typename T>
class Vector2 
{

public:
    
    Vector2()
        : m_X(0)
        , m_Y(0)
    {
    }

    Vector2(T x, T y)
        : m_X(x)
        , m_Y(y)
    {
    }

    Vector2(const Vector2& other)
        : m_X(other.m_X)
        , m_Y(other.m_Y)
    {

    }

    // Cast constructor
    template<typename U>
    Vector2(const Vector2<U>& other)
    {
        m_X = (T)other.GetX();
        m_Y = (T)other.GetY();
    }

    // Getter methods
    const T& GetX() const { return m_X; }
    const T& GetY() const { return m_Y; }

    // Setter methods
    void SetX(T x) { m_X = x; }
    void SetY(T y) { m_Y = y; }
    void SetXY(T x, T y) { m_X = x; m_Y = y; }

    // Magnitude of the vector
    double Magnitude() const 
    {
        return std::sqrt((m_X * m_X) + (m_Y * m_Y));
    }

    // Normalize
    Vector2 Normalize() const
    {
        double mag = Magnitude();
        if (mag != 0) 
        {
            return Vector2(m_X / mag, m_X / mag);
        }

        return *this;
    }

    // Addition operator
    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(m_X + other.m_X, m_Y + other.m_Y);
    }

    // Addition assignment operator
    void operator+=(const Vector2& other)
    {
        m_X += other.m_X;
        m_Y += other.m_Y;
    }

    // Subtraction operator
    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(m_X - other.m_X, m_Y - other.m_Y);
    }

    // Multiplication operator (scalar)
    Vector2 operator*(T scalar) const
    {
        return Vector2(m_X * scalar, m_Y * scalar);
    }

    // Division operator (scalar)
    Vector2 operator/(T scalar) const
    {
        if (scalar != 0) 
        {
            return Vector2(m_X / scalar, m_Y / scalar);
        }

        return *this;
    }

    Vector2& operator=(const Vector2& other)
    {
        m_X = other.m_X;
        m_Y = other.m_Y;
        return *this;
    }

    bool operator==(const Vector2& other) const
    {
        return m_X == other.m_X && m_Y == other.m_Y;
    }

    bool operator!=(const Vector2& other) const
    {
        return m_X != other.m_X || m_Y != other.m_Y;
    }

    bool operator<(const Vector2& other) const
    {
        // Compare magnitudes first
        double magLeft = Magnitude();
        double magRight = other.Magnitude();
        if (magLeft != magRight) 
        {
            return magLeft < magRight;
        }

        if (m_X != other.m_X)
        {
            return m_X < other.m_X;
        }

        return m_Y < other.m_Y;
    }

    // Dot Product
    T Dot(const Vector2& other) const
    {
        return (m_X * other.m_X) + (m_Y * other.m_Y);
    }

    // Cross Product
    T Cross(const Vector2& other) const
    {
        return (m_X * other.m_X) - (m_Y * other.m_X);
    }

    // Clamp
    void Clamp(const Vector2& min, const Vector2& max)
    {
        // in case min > max
        T clampedMinX = std::min<T>(min.m_X, max.m_X);
        T clampedMaxX = std::max<T>(min.m_X, max.m_X);
        T clampedMinY = std::min<T>(min.m_Y, max.m_Y);
        T clampedMaxY = std::max<T>(min.m_Y, max.m_Y);

        m_X = std::max<T>(clampedMinX, std::min<T>(m_X, clampedMaxX));
        m_Y = std::max<T>(clampedMinY, std::min<T>(m_Y, clampedMaxY));
    }

    // Zero
    static const Vector2 Zero() { return Vector2(0, 0); }

    // GoogleTest output
    friend void PrintTo(const Vector2& point, std::ostream* os) {
        *os << "(" << point.m_X << "," << point.m_Y << ")";
    }

    using ValueType = T;
    static constexpr std::size_t SizeOfT = sizeof(T);

private:

    T m_X;
    T m_Y;
};

using Vector2D = Vector2<double>;
using PositionScalar = int16_t;
using PositionVector2D = Vector2<PositionScalar>;

} // namespace raid