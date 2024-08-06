#include "engine/pch.h"
#include "engine/game/damage.h"
#include "engine/game/game_events.h"

namespace raid
{
	
void DamageReceiver::Init()
{
	PowerComponent* powers = m_Parent.GetComponent<PowerComponent>();
	if (powers)
	{
		m_Health = powers->GetPower<PowerType::Health>();
	}
}

void DamageReceiver::ApplyDamage(const DamageEvent& evt)
{
	if (!m_Health)
	{
		return;
	}

	m_Health->Increment(-evt.ActualDamage);

	if (m_Health->IsEmpty())
	{
		
	}
}

double DamageCalculator::CalculateArmorResistPercent(double enemy_level, double armor)
{
	const double BaseArmorResist = 5;

	const double enemy_level_ratio = 5 * enemy_level;
	const double armor_ratio = 100;

	double DR = armor / (enemy_level + armor_ratio + armor) * 100;
	return DR;
}

double DamageCalculator::CalculateResistPercent(double enemy_level, double resist)
{
	const double BaseArmorResist = 0;

	const double enemy_level_ratio = 5 * enemy_level;
	const double mr_ratio = 50;

	double DR = resist / (enemy_level + mr_ratio + resist) * 100;
	return DR;
}

} // namespace raid