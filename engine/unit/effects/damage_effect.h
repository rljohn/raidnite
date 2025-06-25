#pragma once

#include "engine/unit/buff.h"
#include "engine/game/damage.h"

namespace raid
{
	struct BE_Damage : IBuffEffect
	{
		void Init(EntityId source, AbilityId spell, DamageType type, double value);

		void OnAdd(Entity& owner);
		void OnRemove() override;

	protected:

		bool GetLevel(EntityId id, double& out_value) const;

		EntityId m_Source;
		EntityId m_TargetId;
		AbilityId m_SpellId;
		DamageType m_DamageType;
		double m_Value;
	};
}
