#pragma once

#include "engine/entity/component.h"
#include "engine/entity/entity.h"
#include "engine/system/delegate.h"
#include "engine/system/modifier.h"
#include "engine/types.h"
#include "engine/unit/buff.h"

namespace raid
{

enum class PowerType
{
	Invalid = -1,
	Health,
	Mana,
	Rage,
	Energy
};

enum class PowerField
{
	Min,
	Max
};

struct IPowerModifier : public IModifier
{
	IPowerModifier() : IModifier() { }
	IPowerModifier(double modifier) : IModifier(modifier) { }

	virtual PowerField GetPowerField() const = 0;
};

template<ModifierType MOD, PowerField FIELD>
struct PowerModifier : public IPowerModifier
{
	PowerModifier() : IPowerModifier() { }
	PowerModifier(double modifier) : IPowerModifier(modifier) { }

	ModifierType GetModifierType() const override { return MOD; }
	PowerField GetPowerField() const override { return FIELD; }
};

class PowerValue
{
public:

	PowerValue()
		: m_CurrentValue(0)
		, m_MinValue(0)
		, m_MaxValue(0)
	{
	}

	PowerValue(double max)
		: m_CurrentValue(max)
		, m_MinValue(0)
		, m_MaxValue(max)
	{
	}

	// Init
	void Init(double min, double max);

	// Set Functions
	bool SetCurrent(double current);
	void SetMax(double max);
	void SetMin(double min);

	// Get Functions
	double GetCurrent() const { return m_CurrentValue; }
	double GetMin() const { return m_MinValue; }
	double GetMax() const { return m_MaxValue; }
	double GetPercent() const;

	// Queries
	bool IsEmpty() const { return m_CurrentValue == m_MinValue; }
	bool IsFull() const { return m_CurrentValue == m_MaxValue; }

protected:

	double Clamp(double newValue) const;

	double m_CurrentValue;
	double m_MinValue;
	double m_MaxValue;
	
};

class IPower
{
public:

	virtual PowerType GetPowerType() const = 0;

	// Base Values
	void SetBaseValues(double min, double max)
	{
		m_BaseValue.SetMin(min);
		m_BaseValue.SetMax(max);
		m_NeedsCalculation = true;
	}

	// Clamped set of current value
	void SetCurrent(double current);
	void SetPercent(double percent);
	
	// Getters
	double GetCurrent() const { return m_ModifiedValue.GetCurrent(); }
	double GetMin() const  {  return m_ModifiedValue.GetMin(); }
	double GetMax() const  {  return m_ModifiedValue.GetMax(); }
	double GetPercent() const { return m_ModifiedValue.GetPercent(); }

	// Utilities
	void ReFill() { m_ModifiedValue.SetCurrent(m_ModifiedValue.GetMax()); }
	bool IsEmpty() const { return m_ModifiedValue.GetCurrent() == m_ModifiedValue.GetMin(); }
	void SetEmpty() { m_ModifiedValue.SetCurrent(m_ModifiedValue.GetMin()); }
	void Increment(double value);

	// Modifiers
	void AddModifier(IPowerModifier* modifier);
	void RemoveModifier(IPowerModifier* modifier);
	void SortModifiers();

	// Dirty Flag - Recalculate
	bool NeedsRecalculate() const { return m_NeedsCalculation; }
	void Recalculate();

	// Change Events
	using PowerChangedDelegate = Delegate<double /* min */, double /* max */, double /* current */>;
	PowerChangedDelegate& PowerChanged() { return m_PowerChanged; }

protected:

	// Internal Setters
	void SetMax(double max);
	void SetMin(double min);

	PowerValue m_BaseValue;
	PowerValue m_ModifiedValue;

	std::vector<IPowerModifier*> m_Modifiers;

	PowerChangedDelegate m_PowerChanged;
	bool m_NeedsCalculation = false;
};

template<PowerType T>
class Power : public IPower
{
public:

	Power() : IPower()
	{
	}

	Power(double min, double max)
		: IPower() 
	{
		Init(min, max);
	}

	Power(double max)
		: IPower()
	{
		Init(0, max);
	}

	void Init(double min, double max)
	{
		m_BaseValue.Init(min, max);
		m_ModifiedValue.Init(min, max);
	}

	// IPower
	PowerType GetPowerType() const override { return T; }

};

class PowerComponent : public Component
{
public:

	PowerComponent(Entity& parent)
		: Component(parent)
	{
	}

	template<PowerType T>
	void AddPower(double max)
	{
		m_Powers.emplace(T, new Power<T>(max));
	}

	template<PowerType T>
	IPower* GetPower()
	{
		return GetMapEntry(m_Powers, T);
	}

	void Update(const GameFrame& /* frame */) override;

private:

	using Powers = std::unordered_map<PowerType, IPower*>;
	Powers m_Powers;
};

// Define all of our common power types
using Health = Power<PowerType::Health>;
using Mana = Power<PowerType::Mana>;
using Rage = Power<PowerType::Rage>;
using Energy = Power<PowerType::Energy>;

// Helper that holds a reference to a Power ptr.
// Setup with the given enity.
struct PowerReference
{
	template<PowerType T>
	void Setup(Entity& entity)
	{
		if (PowerComponent* powers = entity.GetComponent<PowerComponent>())
		{
			m_Power = powers->GetPower<T>();
		}
	}

	IPower* m_Power = nullptr;
};

// Modifiers for Power
template <PowerType POWER, ModifierType MOD, PowerField FIELD>
class BE_PowerModifier : public IBuffEffect, public PowerModifier<MOD, FIELD>, PowerReference
{
public:

	using PowerModifier<MOD, FIELD>::GetModifier;

	PowerType GetPowerType() const { POWER; }

	BE_PowerModifier()
		: PowerModifier<MOD, FIELD>()
	{
	}

	BE_PowerModifier(double modifier)
		: PowerModifier<MOD, FIELD>(modifier)
	{
	}

	void OnAdd(Entity& entity) override
	{
		PowerReference::Setup<POWER>(entity);

		if (m_Power)
		{
			m_Power->AddModifier(this);
		}
	}

	void OnRemove() override
	{
		if (m_Power)
		{
			m_Power->RemoveModifier(this);
		}
	}
};

template <PowerType T>
class BE_PowerRestoreOnRemoval : public IBuffEffect, public PowerReference
{
public:

	void OnAdd(Entity& entity) override
	{
		PowerReference::Setup<T>(entity);
		CachePercentage();
	}

	void OnUpdate(GameFrame& frame) override
	{
		CachePercentage();
	}

	void CachePercentage()
	{
		if (m_Power)
		{
			m_RestorePercentage = m_Power->GetPercent();
		}
	}

	void OnRemove() override
	{
		if (m_Power)
		{
			m_Power->Recalculate();
			m_Power->SetPercent(m_RestorePercentage);
		}
	}

private:

	double m_RestorePercentage;
};

// Declare common power modifiers
#define DECLARE_POWER_BUFFS(Power) \
    using BE_##Power##MultiplyDivideMin = BE_PowerModifier<PowerType::Power, ModifierType::MultiplyDivide, PowerField::Min>; \
	using BE_##Power##MultiplyDivideMax = BE_PowerModifier<PowerType::Power, ModifierType::MultiplyDivide, PowerField::Max>; \
	using BE_##Power##AddSubtractMin = BE_PowerModifier<PowerType::Power, ModifierType::AddSubtract, PowerField::Min>; \
	using BE_##Power##AddSubtractMax = BE_PowerModifier<PowerType::Power, ModifierType::AddSubtract, PowerField::Max>;

	DECLARE_POWER_BUFFS(Health)
	DECLARE_POWER_BUFFS(Mana)
	DECLARE_POWER_BUFFS(Rage)
	DECLARE_POWER_BUFFS(Energy)

#undef DECLARE_POWER_BUFFS

} // namespace raid