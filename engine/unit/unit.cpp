#include "engine/pch.h"
#include "unit.h"

#include "engine/unit/attribute.h"
#include "engine/game/world.h"
#include "engine/map/tile.h"

namespace raid
{

Unit::Unit()
	: m_Transform(AddComponent<TransformComponent>())
	, m_Powers(AddComponent<PowerComponent>())
	, m_Attributes(AddComponent<AttributesComponent>())
	, m_Buffs(AddComponent<BuffsComponent>())
	, m_DamageReceiver(AddComponent<DamageReceiver>())
	, m_AggroTable(AddComponent<AggroTable>())
	, m_Name(AddComponent<NameComponent>())
	, m_Targeting(AddComponent<TargetingComponent>())
	, m_StateMachine(AddComponent<StateMachineComponent>())
	, m_Movement(AddComponent<MovementComponent>(m_Transform))
	, m_AiController(nullptr)
{
}

void Unit::BeginFrame()
{
	m_Attributes.BeginFrame();
}

void Unit::UpdateFrame(GameFrame& frame)
{
	Entity::Update(frame);
}

void Unit::EndFrame()
{

}

void Unit::OnGameEvent(const GameEvent& evt)
{
	Entity::OnGameEvent(evt);
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

bool Unit::OccupyTile(Tile& t)
{
	mapAssert(!t.IsOccupied());
	if (t.IsOccupied())
		return false;

	t.SetOccupant(this);
	m_Transform.SetOccupyingTile(t.GetPosition());
	return true;
}

bool Unit::IsInCombat() const
{
	return !m_AggroTable.Empty();
}

} // namespace raid