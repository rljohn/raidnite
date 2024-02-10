#pragma once

#include "engine/types.h"
#include "engine/game/damage.h"
#include "engine/game/game_events.h"
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

	virtual void KillEntity(Entity* e) = 0;

	using GameEventDelegate = Delegate<GameEvent*>;
	GameEventDelegate& GameEventDlgt() { return m_GameEventDlgt; }

protected:

	GameEventDelegate m_GameEventDlgt;
};

class CombatSystem : public ICombatSystem
{
public:

	void BuildDamageEvent(const DamageParams& damage) override;
	void KillEntity(Entity* e) override;
};

// Utility (mostly for testing) to ensure the combat system is set and destroyed.
class CombatSystemRAII
{
public:

	CombatSystemRAII();
	~CombatSystemRAII();

	CombatSystem Instance;
};

} // namespace raid