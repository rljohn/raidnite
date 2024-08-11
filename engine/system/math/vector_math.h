#pragma once

#include "vector2.h"
#include "vector3.h"

namespace raid {
namespace VectorMath {

	// Conversion
	Vector2D FromVector3(const Vector3D& other);
	Vector3D FromVector2(const Vector2D& other);
	Location FromPosition(const Position& position);

	// Distance
	double DistanceBetween(const Vector2D& a, const Vector2D& b);
	double DistanceBetween(const Vector3D& a, const Vector3D& b);
	
	// Translation
	Vector2D TranslateTo(const Vector2D& start, const Vector2D& end, double distance);
	Vector3D TranslateTo(const Vector3D& start, const Vector3D& end, double distance);
	 
} // VectorMath
} // raid