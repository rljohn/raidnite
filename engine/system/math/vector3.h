#pragma once

#pragma once

#include <cmath>

namespace raid
{

template<typename T>
class Vector3
{

public:

    Vector3()
        : m_X(0.0)
        , m_Y(0.0)
        , m_Z(0.0)
    {
    }

    Vector3(T x, T y, T z)
        : m_X(x)
        , m_Y(y)
        , m_Z(z)
    {
    }

    // Getter methods
    T GetX() const { return m_X; }
    T GetY() const { return m_Y; }
    T GetZ() const { return m_Z; }

    // Setter methods
    void SetX(T x) { m_X = x; }
    void SetY(T y) { m_Y = y; }
    void SetZ(T z) { m_Z = z; }

    // Magnitude of the vector
    double Magnitude() const
    {
        return std::sqrt((m_X * m_X) + (m_Y * m_Y) + (m_Z * m_Z));
    }

    // Normalize
    Vector3 Normalize() const
    {
        double mag = Magnitude();
        if (mag != 0)
        {
            return Vector3(m_X / mag, m_Y / mag, m_Z / mag);
        }

        return *this;
    }

    // Addition operator
    Vector3 operator+(const Vector3& other) const
    {
        return Vector3(m_X + other.m_X, m_Y + other.m_Y, m_Z + other.m_Z);
    }

    // Subtraction operator
    Vector3 operator-(const Vector3& other) const
    {
        return Vector3(m_X - other.m_X, m_Y - other.m_Y, m_Z - other.m_Z);
    }

    // Multiplication operator (scalar)
    Vector3 operator*(T scalar) const
    {
        return Vector3(m_X * scalar, m_Y * scalar, m_Z * scalar);
    }

    // Division operator (scalar)
    Vector3 operator/(T scalar) const
    {
        if (scalar != 0)
        {
            return Vector3(m_X / scalar, m_Y / scalar, m_Z / scalar);
        }

        return *this;
    }

    bool operator==(const Vector3& other) const
    {
        return m_X == other.m_X && m_Y == other.m_Y && m_Z == other.m_Z;
    }

    bool operator<(const Vector3& other) const
    {
        // Compare magnitudes first
        double magLeft = Magnitude();
        double magRight = other.Magnitude();
        if (magLeft != magRight)
        {
            return magLeft < magRight;
        }

        // If magnitudes are equal, compare individual components
        if (m_X != other.m_X) 
        {
            return m_X < other.m_X;
        }

        if (m_Y != other.m_Y) 
        {
            return m_Y < other.m_Y;
        }

        return m_Z < other.m_Z;
    }

    // Dot Product
    T Dot(const Vector3& other) const
    {
        return (m_X * other.m_X) + (m_Y * other.m_Y) + (m_Z * other.m_Z);
    }

    // Cross Product
    Vector3 Cross(const Vector3& other) const
    {
        T x = (m_Y * other.m_Z) - (m_Z * other.m_Y);
        T y = (m_Z * other.m_X) - (m_X * other.m_Z);
        T z = (m_X * other.m_Y) - (m_Y * other.m_X);

        return Vector3(x, y, z);
    }

private:

    T m_X;
    T m_Y;
    T m_Z;
};

using IntVector3 = Vector3<int>;
using Vector3D = Vector3<double>;

} // namespace raid