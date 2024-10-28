#pragma once

#include <stdint.h>
#include "engine/system/multikeymap.h"

namespace raid
{

using FactionId = uint8_t;

enum class FactionRelationship
{
	Invalid,
	Friendly,
	Hostile,
	Neutral
};

class IFactionManager
{
public:

	virtual FactionRelationship GetRelationship(FactionId a, FactionId b) const = 0;
	virtual void SetRelationship(FactionId a, FactionId b, FactionRelationship r) = 0;

	virtual bool IsFriendly(FactionId a, FactionId b) const = 0;
	virtual bool IsNeutral(FactionId a, FactionId b) const = 0;
	virtual bool IsHostile(FactionId a, FactionId b) const = 0;
};

class FactionManager : public IFactionManager
{
public:

	FactionRelationship GetRelationship(FactionId a, FactionId b) const override;
	void SetRelationship(FactionId a, FactionId b, FactionRelationship r) override;

	bool IsFriendly(FactionId a, FactionId b) const override;
	bool IsNeutral(FactionId a, FactionId b) const override;
	bool IsHostile(FactionId a, FactionId b) const override;

	CombinedKeyMap<FactionId, FactionRelationship> m_Relationships;
};

}