#include "engine/pch.h"
#include "unit.h"

#include "engine/unit/attribute.h"
#include "engine/game/world.h"

namespace raid
{

Unit::Unit()
	: m_Powers(AddComponent<PowerComponent>())
	, m_Attributes(AddComponent<AttributesComponent>())
	, m_Buffs(AddComponent<BuffsComponent>())
	, m_DamageReceiver(AddComponent<DamageReceiver>())
{

}

void Unit::BeginFrame()
{
	m_Attributes.BeginFrame();
}

void Unit::UpdateFrame(GameFrame& frame)
{
	m_Buffs.Update(frame);
}

void Unit::EndFrame()
{

}

void Unit::AddBuff(Buff* buff)
{
	m_Buffs.AddBuff(buff);
}

void Unit::CleanseBuff(Buff* buff)
{
	m_Buffs.CleanseBuff(buff);
}

void Unit::RemoveBuff(Buff* buff)
{
	m_Buffs.RemoveBuff(buff);
}

} // namespace raid