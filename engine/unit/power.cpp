#include "engine/pch.h"
#include "power.h"

#include "engine/system/container.h"
#include <algorithm>

namespace raid
{

void PowerValue::Init(double min, double max)
{
	m_MinValue = min;
	m_MaxValue = max;
	m_CurrentValue = max;
}

bool PowerValue::SetCurrent(double value)
{
	double newValue = Clamp(value);

	if (m_CurrentValue != newValue)
	{
		m_CurrentValue = newValue;
		return true;
	}

	return false;
}

void PowerValue::SetMax(double max)
{
	m_MaxValue = max;
}

void PowerValue::SetMin(double min)
{
	m_MinValue = min;
}

double PowerValue::GetPercent() const
{
	double range = m_MaxValue - m_MinValue;
	double fill = m_CurrentValue - m_MinValue;

	if (range > 0)
	{
		return (fill / range) * 100.0;
	}
	else
	{
		return 0;
	}
}

double PowerValue::Clamp(double value) const
{
	return std::clamp(value, m_MinValue, m_MaxValue);
}

void IPower::SetMax(double max)
{ 
	m_ModifiedValue.SetMax(max);
}

void IPower::SetMin(double min)
{ 
	m_ModifiedValue.SetMin(min);
}

void IPower::SetCurrent(double current)
{ 
	if (m_ModifiedValue.SetCurrent(current))
	{
		m_PowerChanged.Invoke(m_ModifiedValue.GetMin(), m_ModifiedValue.GetMax(), m_ModifiedValue.GetCurrent());
	}
}

void IPower::SetPercent(double percent)
{
	percent = std::max(percent, 0.0);
	double range = GetMax() - GetMin();
	double newCurrent = GetMin() + (percent * range);
	SetCurrent(newCurrent);
}

void IPower::Recalculate()
{
	double current = GetCurrent();

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

void IPower::Increment(double value)
{
	double newValue = m_ModifiedValue.GetCurrent() + value;
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
	VectorRemove(m_Modifiers, modifier);
	SortModifiers();
	m_NeedsCalculation = true;
}

void IPower::SortModifiers()
{
	std::sort(m_Modifiers.begin(), m_Modifiers.end());
}

///////////////////////////////////

void PowerComponent::Update(const GameFrame& /* frame */)
{
	for (auto& pair : m_Powers)
	{
		if (pair.second->NeedsRecalculate())
		{
			pair.second->Recalculate();
		}
	}
}

} // namespace raid