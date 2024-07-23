#include "engine/pch.h"
#include "vector2.h"

namespace raid
{

Vector2D VectorMath::TranslateTo(const Vector2D& start, const Vector2D& end, double distance)
{
	Vector2 direction = (end - start);
	
	double mag = direction.Magnitude();
	if (mag <= distance)
	{
		return end;
	}

	Vector2 norm = direction / mag;
	Vector2 offset = norm * distance;
	return start + offset;
}

} // namespace raid
