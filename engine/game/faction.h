#pragma once

#include <stdint.h>

namespace raid
{

using FactionId = uint8_t;

class Faction
{
public:

	enum class Relationship
	{
		Friendly,
		Hostile,
		Neutral
	};

	Relationship GetRelationship(FactionId a, FactionId b) const;

	bool IsFriendly(FactionId a, FactionId b);
	bool IsNeutral(FactionId a, FactionId b);
	bool IsHostile(FactionId a, FactionId b);
};

}