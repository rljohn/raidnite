#pragma once

#include <map>

#include "engine/unit/buff.h"
#include "engine/system/modifier.h"

namespace raid
{

enum class AttributeType
{
	Invalid = -1,
	Stamina,
	Intellect,
	Strength,
	Agility,
	Armor,
};

struct IAttribute
{
	virtual AttributeType GetAttributeType() const = 0;

	float BaseValue;
	float CurrentValue;
};

template <AttributeType T>
struct Attribute : IAttribute
{
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

	template<AttributeType T>
	void AddAttribute(float max)
	{
		m_Attributes.emplace(T, new Attribute<T>());
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

using Stamina = Attribute<AttributeType::Stamina>;
using Intellect = Attribute<AttributeType::Intellect>;
using Strength = Attribute<AttributeType::Strength>;
using Agility = Attribute<AttributeType::Agility>;
using Armor = Attribute<AttributeType::Armor>;

template <AttributeType ATTR, ModifierType MOD>
struct BE_AttributeModifier : IBuffEffect, Modifier<MOD>
{
	AttributeType GetAttributeType() const { ATTR; }
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