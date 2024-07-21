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
	IModifier(double modifier) : m_Modifier(modifier) { }

	double Apply(double value) const
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

	void SetModifier(double modifier)
	{
		m_Modifier = modifier; 
	}
	
	double GetModifier() const
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

	double m_Modifier;
};

template <ModifierType M>
struct Modifier : IModifier
{
	Modifier() : IModifier() {}
	Modifier(double modifier) : IModifier(modifier) { }

	ModifierType GetModifierType() const override { return M; }
};

} // namespace raid
