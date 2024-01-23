#pragma once

namespace raid
{

enum ModifierType
{
	MultiplyDivide,
	AddSubtract,
};

struct IModifier
{
	virtual ModifierType GetModifierType() const = 0;
	
	IModifier() : m_Modifier(0) {}
	IModifier(float modifier) : m_Modifier(modifier) { }

	float Apply(float value) const
	{
		if (GetModifierType() == ModifierType::AddSubtract)
		{
			return value + GetModifier();
		}
		else if (GetModifierType() == ModifierType::MultiplyDivide)
		{
			return value * GetModifier();
		}

		return value;
	}

	void SetModifier(float modifier) 
	{
		m_Modifier = modifier; 
	}
	
	float GetModifier() const
	{ 
		return m_Modifier; 
	}

	bool operator<(const IModifier& other)
	{
		if (GetModifierType() < other.GetModifierType())
		{
			return true;
		}
		else
		{
			return GetModifier() < other.GetModifier();
		}
	}

private:

	float m_Modifier;
};

template <ModifierType M>
struct Modifier : IModifier
{
	ModifierType GetModifierType() const override { return M; }
};

} // namespace raid
