#include "engine/pch.h"
#include "faction.h"

namespace raid
{
	FactionRelationship FactionManager::GetRelationship(FactionId a, FactionId b) const
	{
		if (a == b)
			return FactionRelationship::Friendly;

		const FactionRelationship* result = m_Relationships.Get(a, b);
		return result ? *result : FactionRelationship::Invalid;
	}

	void FactionManager::SetRelationship(FactionId a, FactionId b, FactionRelationship r)
	{
		if (a == b)
			return;

		m_Relationships.Set(a, b, r);
	}

	bool FactionManager::IsFriendly(FactionId a, FactionId b) const
	{
		return GetRelationship(a, b) == FactionRelationship::Friendly;
	}

	bool FactionManager::IsNeutral(FactionId a, FactionId b) const
	{
		FactionRelationship r = GetRelationship(a, b);
		return (r == FactionRelationship::Neutral) || (r == FactionRelationship::Invalid);
	}

	bool FactionManager::IsHostile(FactionId a, FactionId b) const
	{
		return GetRelationship(a, b) == FactionRelationship::Hostile;
	}

} // namespace raid