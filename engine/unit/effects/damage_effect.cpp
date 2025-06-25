#include "engine/pch.h"
#include "damage_effect.h"

#include "engine/game/damage.h"
#include "engine/game/combat.h"
#include "engine/entity/entity.h"
#include "engine/unit/unit.h"
#include "engine/unit/unit_utils.h"

namespace raid
{
	void BE_Damage::Init(EntityId source, AbilityId spell, DamageType type, double value)
	{
		m_Source = source;
		m_SpellId = spell;
		m_DamageType = type;
		m_Value = value;
	}

	bool BE_Damage::GetLevel(EntityId id, double& out_value) const
	{
		if (IEntityManager* locator = Game::GetEntityManager())
		{
			Entity* source_entity = locator->FindEntity(m_Source);
			if (source_entity)
			{
				return UnitStatics::GetUnitLevel(*source_entity, out_value);
			}
		}

		return false;
	}

	void BE_Damage::OnAdd(Entity& owner)
	{
		m_TargetId = owner.GetId();
	}

	void BE_Damage::OnRemove()
	{
		if (ICombatSystem* system = Game::GetCombatSystem())
		{
			ICombatSystem::DamageParams dmg;
			dmg.Source = m_Source;
			dmg.Target = m_TargetId;
			dmg.Spell = m_SpellId;
			dmg.Type = m_DamageType;
			dmg.Value = m_Value;

			system->BuildDamageEvent(dmg);
		}
	}

} // namespace raid
