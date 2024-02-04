#include "engine/pch.h"
#include "buff.h"
#include "engine/unit/unit.h"
#include "engine/game/world.h"

namespace raid
{

/////////////////////////////////////////////////
// Buff
/////////////////////////////////////////////////

void Buff::Begin(const TimeStamp& current)
{
	m_StartTime = current;
}

bool Buff::IsExpired() const
{
	return m_TimeRemaining.count() < 0;
}

void Buff::AddEffect(IBuffEffect* effect)
{
	m_Effects.push_back(effect);
}

void Buff::SetDuration(const Duration& duration)
{
	m_Duration = duration;
}

void Buff::OnAdd(Entity& owner)
{
	for (IBuffEffect* effect : m_Effects)
	{
		effect->OnAdd(owner);
	}
}

void Buff::OnUpdate(GameFrame& frame)
{
	m_TimeRemaining -= frame.DeltaTime;

	for (IBuffEffect* effect : m_Effects)
	{
		effect->OnUpdate(frame);
	}
}

void Buff::OnRemove(RemoveReason reason)
{
	for (IBuffEffect* effect : m_Effects)
	{
		effect->OnRemove();
	}
}

/////////////////////////////////////////////////
// Buff Component
/////////////////////////////////////////////////

void BuffsComponent::Update(GameFrame& frame)
{
	for (auto it = m_Buffs.begin(); it != m_Buffs.end(); it++)
	{
		Buff* buff = *it;
		if (buff)
		{
			buff->OnUpdate(frame);

			if (buff->IsExpired())
			{
				buff->OnRemove(Buff::RemoveReason::Expired);
				it = m_Buffs.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void BuffsComponent::AddBuff(Buff* buff)
{
	m_Buffs.push_back(buff);
	buff->OnAdd(m_Parent);
}

void BuffsComponent::CleanseBuff(Buff* buff)
{
	RemoveFromVector(m_Buffs, buff);
	buff->OnRemove(Buff::RemoveReason::Cleansed);
}

void BuffsComponent::RemoveBuff(Buff* buff)
{
	RemoveFromVector(m_Buffs, buff);
	buff->OnRemove(Buff::RemoveReason::Removed);
}

void BuffsComponent::ExpireBuff(Buff* buff)
{
	RemoveFromVector(m_Buffs, buff);
	buff->OnRemove(Buff::RemoveReason::Expired);
}

} // namespace raid

