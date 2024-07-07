#pragma once

#include <cmath>

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

    // Getter methods
    const T& GetX() const { return m_X; }
    const T& GetY() const { return m_Y; }

    // Setter methods
    void SetX(T x) { m_X = x; }
    void SetY(T y) { m_Y = y; }

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
            return Vector2D(m_X / mag, m_X / mag);
        }

        return *this;
    }

    // Addition operator
    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(m_X + other.m_X, m_Y + other.m_Y);
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

private:

    T m_X;
    T m_Y;
};

using IntVector2D = Vector2<int>;
using Vector2D = Vector2<double>;

} // namespace raid