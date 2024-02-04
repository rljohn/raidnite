#include "engine/pch.h"
#include "power.h"

#include <algorithm>

namespace raid
{

void PowerValue::Init(float min, float max)
{
	m_MinValue = min;
	m_MaxValue = max;
	m_CurrentValue = max;
}

bool PowerValue::SetCurrent(float value)
{
	float newValue = Clamp(value);

	if (m_CurrentValue != newValue)
	{
		m_CurrentValue = newValue;
		return true;
	}

	return false;
}

void PowerValue::SetMax(float max)
{
	m_MaxValue = max;
}

void PowerValue::SetMin(float min)
{
	m_MinValue = min;
}

float PowerValue::GetPercent() const
{
	float range = m_MaxValue - m_MinValue;
	float fill = m_CurrentValue - m_MinValue;

	if (range > 0)
	{
		return (fill / range) * 100.0f;
	}
	else
	{
		return 0;
	}
}

float PowerValue::Clamp(float value) const
{
	return std::clamp(value, m_MinValue, m_MaxValue);
}

void IPower::SetMax(float max)
{ 
	m_ModifiedValue.SetMax(max);
}

void IPower::SetMin(float min)
{ 
	m_ModifiedValue.SetMin(min);
}

void IPower::SetCurrent(float current)
{ 
	if (m_ModifiedValue.SetCurrent(current))
	{
		m_PowerChanged.Invoke(m_ModifiedValue.GetMin(), m_ModifiedValue.GetMax(), m_ModifiedValue.GetCurrent());
	}
}

void IPower::SetPercent(float percent) 
{
	percent = std::max(percent, 0.0f);
	float range = GetMax() - GetMin();
	float newCurrent = GetMin() + (percent * range);
	SetCurrent(newCurrent);
}

void IPower::Recalculate()
{
	float current = GetCurrent();

	// Restore base values for min/end
	SetMin(m_BaseValue.GetMin());
	SetMax(m_BaseValue.GetMax());

	// Apply modifiers
	for (IPowerModifier* mod : m_Modifiers)
	{
		switch (mod->GetPowerField())
		{
		case PowerField::Min:
			SetMin(mod->Apply(GetMin()));
			break;
		case PowerField::Max:
			SetMax(mod->Apply(GetMax()));
			break;
		}
	}

	// Attempt to keep the current value, but clamp it to the new min/max.
	SetCurrent(current);
	m_NeedsCalculation = false;
}

void IPower::Increment(float value)
{
	float newValue = m_ModifiedValue.GetCurrent() + value;
	m_ModifiedValue.SetCurrent(newValue);
}

void IPower::AddModifier(IPowerModifier* modifier)
{
	m_Modifiers.push_back(modifier);
	SortModifiers();
	m_NeedsCalculation = true;
}
 
void IPower::RemoveModifier(IPowerModifier* modifier)
{
	RemoveFromVector(m_Modifiers, modifier);
	SortModifiers();
	m_NeedsCalculation = true;
}

void IPower::SortModifiers()
{
	std::sort(m_Modifiers.begin(), m_Modifiers.end());
}

} // namespace raid