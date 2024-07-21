#include "engine/pch.h"
#include "engine/unit/attribute.h"

namespace raid
{

////////////////////////
// IAttribute
////////////////////////

IAttribute::IAttribute()
	: IAttribute(0)
{

}

IAttribute::IAttribute(double value)
	: m_BaseValue(value)
	, m_CurrentValue(value)
	, m_NeedsCalculation(false)
{

}

void IAttribute::Recalculate()
{
	m_CurrentValue = m_BaseValue;

	// Apply modifiers
	for (IModifier* mod : m_Modifiers)
	{
		m_CurrentValue = mod->Apply(m_CurrentValue);
	}

	m_NeedsCalculation = false;
}

void IAttribute::AddModifier(IModifier* modifier)
{
	m_Modifiers.push_back(modifier);
	SortModifiers();
	m_NeedsCalculation = true;
}

void IAttribute::RemoveModifier(IModifier* modifier)
{
	VectorRemove(m_Modifiers, modifier);
	SortModifiers();
	m_NeedsCalculation = true;
}

void IAttribute::SortModifiers()
{
	std::sort(m_Modifiers.begin(), m_Modifiers.end());
}

////////////////////////
// AttributesComponent
////////////////////////

void AttributesComponent::BeginFrame()
{
}


} // namespace raid