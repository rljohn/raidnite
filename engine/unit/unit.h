#pragma once

#include "engine/entity/entity.h"
#include "engine/unit/attribute.h"
#include "engine/unit/power.h"

namespace raid
{

class Unit : public Entity
{
public:

	Unit();

	// Powers
	template<PowerType T> void AddPower(float max) { return m_Powers.AddPower<T>(max); }
	template<PowerType T> IPower* GetPower() { return m_Powers.GetPower<T>(); }

	// Attributes
	template<AttributeType T> void AddAttribute() { return m_Attributes.AddAttribute<T>(); }
	template<AttributeType T> IPower* GetAttribute() { return m_Attributes.GetAttribute<T>(); }

	// Game tick
	void BeginFrame();
	void UpdateFrame(const GameFrame& frame);
	void EndFrame();

	// Buff System pass through
	void AddBuff(Buff* buff);
	void RemoveBuff(Buff* buff);

private:

	PowerComponent& m_Powers;
	AttributesComponent& m_Attributes;
	BuffsComponent& m_Buffs;
};

class UnitStatics
{
	static void InitUnitHealth(Unit& unit, float min, float max)
	{
		IPower* health = unit.GetPower<PowerType::Health>();
		if (health)
		{
			health->SetBaseValues(min, max);
		}
	}
};

}