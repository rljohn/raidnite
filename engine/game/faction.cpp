#include "engine/pch.h"
#include "faction.h"

namespace raid
{
	Faction::Relationship Faction::GetRelationship(FactionId a, FactionId b) const
	{
		return Faction::Relationship::Neutral;
	}

	bool Faction::IsFriendly(FactionId a, FactionId b)
	{
		return false;
	}

	bool Faction::IsNeutral(FactionId a, FactionId b)
	{
		return false;
	}

	bool Faction::IsHostile(FactionId a, FactionId b)
	{
		return false;
	}

}