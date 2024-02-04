#pragma once

#include "engine/types.h"
#include "engine/game/damage.h"
#include "engine/game/combat_events.h"
#include "engine/system/delegate.h"

namespace raid
{

class Unit;
class World;

class ICombatSystem
{
public:

	struct DamageParams
	{
		EntityId Source = InvalidEntityId;
		float SourceLevel = 0;
		EntityId Target = InvalidEntityId;
		DamageType Type = DamageType::Invalid;
		SpellId Spell = InvalidSpellId;
		float Value = 0;
	};

	virtual void BuildDamageEvent(const DamageParams& damage) = 0;

	using CombatEventDelegate = Delegate<CombatEvent*>;
	CombatEventDelegate& CombatEventDlgt() { return m_CombatEventDlgt; }

protected:

	CombatEventDelegate m_CombatEventDlgt;
};

class CombatSystem : public ICombatSystem
{
public:

	void BuildDamageEvent(const DamageParams& damage) override;
};

class CombatSystemRAII
{

};

} // namespace raid