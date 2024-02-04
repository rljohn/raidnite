#pragma once

#include "engine/game/combat_common.h"
#include "engine/game/combat_events.h"
#include "engine/entity/component.h"
#include "engine/system/delegate.h"
#include "engine/unit/power.h"
#include "engine/unit/attribute.h"

namespace raid
{

	class PowerComponent;

	class DamageReceiver : public Component
	{
	public:

		DamageReceiver(Entity& parent)
			: Component(parent)
			, m_Health(nullptr)
		{
		}

		void Init() override;
		void ApplyDamage(const DamageEvent& evt);

	protected:

		IPower* m_Health;
	};

	class IDamageCalculator
	{
	public:

		virtual float CalculateArmorResistPercent(float enemy_level, float armor) = 0;
		virtual float CalculateResistPercent(float enemy_level, float resist) = 0;
	};

	class DamageCalculator : public IDamageCalculator
	{
		float CalculateArmorResistPercent(float enemy_level, float armor) override;
		float CalculateResistPercent(float enemy_level, float resist) override;
	};

} // namespace
