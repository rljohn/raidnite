#pragma once

#include "engine/entity/entity.h"
#include "engine/game/damage.h"
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
	template<AttributeType T> void AddAttribute(float value = 0) { return m_Attributes.AddAttribute<T>(value); }
	template<AttributeType T> IAttribute* GetAttribute() { return m_Attributes.GetAttribute<T>(); }

	// Game tick
	void BeginFrame();
	void UpdateFrame(GameFrame& frame);
	void EndFrame();

	// Buff System pass through
	void AddBuff(Buff* buff);
	void CleanseBuff(Buff* buff);
	void RemoveBuff(Buff* buff);

private:

	PowerComponent& m_Powers;
	AttributesComponent& m_Attributes;
	BuffsComponent& m_Buffs;
	DamageReceiver& m_DamageReceiver;
};

} // namespace raid