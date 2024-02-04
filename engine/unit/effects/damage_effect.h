#pragma once

#include "engine/unit/buff.h"
#include "engine/game/damage.h"

namespace raid
{
	struct BE_Damage : IBuffEffect
	{
		void Init(EntityId source, SpellId spell, DamageType type, float value);

		void OnAdd(Entity& owner);
		void OnRemove() override;

	protected:

		bool GetLevel(EntityId id, float& out_value) const;

		EntityId m_Source;
		EntityId m_TargetId;
		SpellId m_SpellId;
		DamageType m_DamageType;
		float m_Value;
	};
}
