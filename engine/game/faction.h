#pragma once

#include <stdint.h>

#include "engine/system/multikeymap.h"
#include "engine/localization/localization.h"

namespace raid
{

enum class FactionRelationship
{
	Invalid,
	Friendly,
	Hostile,
	Neutral
};

struct FactionInfo
{
	LocalizationKey DisplayName;
};

class IFactionManager
{
public:

	virtual void RegisterFaction(FactionId id, const FactionInfo& info) = 0;
	virtual void UnregisterFaction(FactionId id) = 0;
	virtual LocalizationKey GetFactionName(FactionId id) const = 0;

	virtual FactionRelationship GetRelationship(FactionId a, FactionId b) const = 0;
	virtual void SetRelationship(FactionId a, FactionId b, FactionRelationship r) = 0;

	virtual bool IsFriendly(FactionId a, FactionId b) const = 0;
	virtual bool IsNeutral(FactionId a, FactionId b) const = 0;
	virtual bool IsNeutralOrHostile(FactionId a, FactionId b) const = 0;
	virtual bool IsHostile(FactionId a, FactionId b) const = 0;
};

class FactionManager : public IFactionManager
{
public:

	void RegisterFaction(FactionId id, const FactionInfo& info) override;
	void UnregisterFaction(FactionId id) override;
	LocalizationKey GetFactionName(FactionId id) const override;

	FactionRelationship GetRelationship(FactionId a, FactionId b) const override;
	void SetRelationship(FactionId a, FactionId b, FactionRelationship r) override;

	bool IsFriendly(FactionId a, FactionId b) const override;
	bool IsNeutral(FactionId a, FactionId b) const override;
	bool IsNeutralOrHostile(FactionId a, FactionId b) const override;
	bool IsHostile(FactionId a, FactionId b) const override;

	CombinedKeyMap<FactionId, FactionRelationship> m_Relationships;
	std::map<FactionId, FactionInfo> m_FactionInfo;
};

// Utility (mostly for testing) to ensure the combat system is set and destroyed.
class FactionManagerRAII
{
public:

	FactionManagerRAII();
	~FactionManagerRAII();

	FactionManager Instance;
};

}