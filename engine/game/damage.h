#pragma once

#include "engine/game/combat_common.h"
#include "engine/entity/component.h"
#include "engine/system/delegate.h"
#include "engine/unit/power.h"
#include "engine/unit/attribute.h"

namespace raid
{

	class PowerComponent;
	struct DamageEvent;

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

		virtual double CalculateArmorResistPercent(double enemy_level, double armor) = 0;
		virtual double CalculateResistPercent(double enemy_level, double resist) = 0;
	};

	class DamageCalculator : public IDamageCalculator
	{
		double CalculateArmorResistPercent(double enemy_level, double armor) override;
		double CalculateResistPercent(double enemy_level, double resist) override;
	};

	// Utility (mostly for testing) to ensure the combat system is set and destroyed.
	class DamageCalculatorRAII
	{
	public:

		DamageCalculatorRAII();
		~DamageCalculatorRAII();

		DamageCalculator Instance;
	};

} // namespace
