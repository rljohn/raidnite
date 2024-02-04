#include "engine/pch.h"
#include "engine/game/damage.h"

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

float DamageCalculator::CalculateArmorResistPercent(float enemy_level, float armor)
{
	const float BaseArmorResist = 5;

	const float enemy_level_ratio = 5 * enemy_level;
	const float armor_ratio = 100;

	float DR = armor / (enemy_level + armor_ratio + armor) * 100;
	return DR;
}

float DamageCalculator::CalculateResistPercent(float enemy_level, float resist)
{
	const float BaseArmorResist = 0;

	const float enemy_level_ratio = 5 * enemy_level;
	const float mr_ratio = 50;

	float DR = resist / (enemy_level + mr_ratio + resist) * 100;
	return DR;
}

} // namespace raid