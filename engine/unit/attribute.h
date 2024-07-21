#pragma once

#include <map>

#include "engine/unit/buff.h"
#include "engine/system/container.h"
#include "engine/system/modifier.h"

namespace raid
{

enum class AttributeType
{
	Invalid = -1,

	// Level
	Level,

	// Movement
	Speed,

	// Base Stats
	Stamina,
	Intellect,
	Strength,
	Agility,

	// Defensive Stats
	DamageResist, // all dmg
	Armor,
	MagicResist,
	FireResist,
	FrostResist,
	EarthResist,

	// Damage Reduction - Elemental
	PhysicalDR,
	FireDR,
	FrostDR,
	EarthDR,

	MAX,
};

struct IAttribute
{
	virtual AttributeType GetAttributeType() const = 0;

	IAttribute();
	IAttribute(double value);

	double GetValue() const { return m_CurrentValue; }
	operator double() const { return GetValue(); }

	// Dirty Flag - Recalculate
	bool NeedsRecalculate() const { return m_NeedsCalculation; }
	void Recalculate();

	// Modifiers
	void AddModifier(IModifier* modifier);
	void RemoveModifier(IModifier* modifier);
	void SortModifiers();

protected:

	std::vector<IModifier*> m_Modifiers;

	double m_BaseValue;
	double m_CurrentValue;

	bool m_NeedsCalculation;
};

template <AttributeType T>
struct Attribute : IAttribute
{
	Attribute(double value)
		: IAttribute(value)
	{
	}

	virtual AttributeType GetAttributeType() const override
	{
		return T;
	}
};

class AttributesComponent : public Component
{
public:

	AttributesComponent(Entity& parent)
		: Component(parent)
	{
	}

	void SetupAttributes()
	{
		AddAttribute<AttributeType::Level>();
		AddAttribute<AttributeType::Speed>();

		AddAttribute<AttributeType::Stamina>();
		AddAttribute<AttributeType::Intellect>();
		AddAttribute<AttributeType::Strength>();
		AddAttribute<AttributeType::Agility>();

		AddAttribute<AttributeType::DamageResist>();
		AddAttribute<AttributeType::MagicResist>();
		AddAttribute<AttributeType::FireResist>();
		AddAttribute<AttributeType::FrostResist>();
		AddAttribute<AttributeType::EarthResist>();

		AddAttribute<AttributeType::PhysicalDR>();
		AddAttribute<AttributeType::FireDR>();
		AddAttribute<AttributeType::FrostDR>();
		AddAttribute<AttributeType::EarthDR>();
	}

	template<AttributeType T>
	void AddAttribute(double value = 0)
	{
		m_Attributes.emplace(T, new Attribute<T>(value));
	}

	template<AttributeType T>
	IAttribute* GetAttribute()
	{
		return GetMapEntry(m_Attributes, T);
	}

	void BeginFrame();

private:

	using Attributes = std::unordered_map<AttributeType, IAttribute*>;
	Attributes m_Attributes;
};

// Define all of our common attribute types
using Stamina = Attribute<AttributeType::Stamina>;
using Intellect = Attribute<AttributeType::Intellect>;
using Strength = Attribute<AttributeType::Strength>;
using Agility = Attribute<AttributeType::Agility>;
using Armor = Attribute<AttributeType::Armor>;

// Helper that holds a reference to a Power ptr.
// Setup with the given enity.
struct AttributeReference
{
	template<AttributeType T>
	void Setup(Entity& entity)
	{
		if (AttributesComponent* attributes = entity.GetComponent<AttributesComponent>())
		{
			m_Attribute = attributes->GetAttribute<T>();
		}
	}

	IAttribute* m_Attribute = nullptr;
};

template <AttributeType ATTR, ModifierType MOD>
struct BE_AttributeModifier : IBuffEffect, Modifier<MOD>, AttributeReference
{
public:

	AttributeType GetAttributeType() const { ATTR; }

	BE_AttributeModifier()
		: Modifier<MOD>()
	{
	}

	BE_AttributeModifier(double modifier)
		: Modifier<MOD>(modifier)
	{
	}

	void OnAdd(Entity& entity) override
	{
		AttributeReference::Setup<ATTR>(entity);

		if (m_Attribute)
		{
			m_Attribute->AddModifier(this);
		}
	}

	void OnRemove() override
	{
		if (m_Attribute)
		{
			m_Attribute->RemoveModifier(this);
		}
	}
};

// Declare common attribute modifiers
#define DECLARE_ATTRIBUTE_BUFFS(ModifierName) \
    using BE_##ModifierName##MultiplyDivide = BE_AttributeModifier<AttributeType::ModifierName, ModifierType::MultiplyDivide>; \
	using BE_##ModifierName##AddSubtract = BE_AttributeModifier<AttributeType::ModifierName, ModifierType::AddSubtract>;

DECLARE_ATTRIBUTE_BUFFS(Stamina)
DECLARE_ATTRIBUTE_BUFFS(Intellect)
DECLARE_ATTRIBUTE_BUFFS(Strength)
DECLARE_ATTRIBUTE_BUFFS(Agility)
DECLARE_ATTRIBUTE_BUFFS(Armor)

#undef DECLARE_ATTRIBUTE_BUFFS

} // namespace raid