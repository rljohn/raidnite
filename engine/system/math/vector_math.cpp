#include "engine/pch.h"
#include "vector_math.h"


namespace raid {
namespace VectorMath {

Vector2D FromVector3(const Vector3D& other)
{
	return Vector2D(other.GetX(), other.GetY());
}

Vector3D FromVector2(const Vector2D& other)
{
	return Vector3D(other.GetX(), other.GetY(), 0);
}

Location FromPosition(const Position& position)
{
	return Location(position.GetX(), position.GetY(), 0);
}

Vector2D TranslateTo(const Vector2D& start, const Vector2D& end, double distance)
{
	Vector2D direction = (end - start);

	double mag = direction.Magnitude();
	if (mag <= distance)
	{
		return end;
	}

	Vector2D norm = direction / mag;
	Vector2D offset = norm * distance;
	return start + offset;
}

Vector3D TranslateTo(const Vector3D& start, const Vector3D& end, double distance)
{
	Vector3D direction = (end - start);

	double mag = direction.Magnitude();
	if (mag <= distance)
	{
		return end;
	}

	Vector3D norm = direction / mag;
	Vector3D offset = norm * distance;
	return start + offset;
}
	 
} // VectorMath
} // raid