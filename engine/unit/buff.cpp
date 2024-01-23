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

void Buff::Update(const GameFrame& frame)
{
	m_TimeRemaining -= frame.DeltaTime;
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

void Buff::OnUpdate()
{
	for (IBuffEffect* effect : m_Effects)
	{
		effect->OnUpdate();
	}
}

void Buff::OnRemove()
{
	for (IBuffEffect* effect : m_Effects)
	{
		effect->OnRemove();
	}
}

/////////////////////////////////////////////////
// Buff System
/////////////////////////////////////////////////

void BuffsComponent::Update(const GameFrame& frame)
{
	for (auto it = m_Buffs.begin(); it != m_Buffs.end(); it++)
	{
		Buff* buff = *it;
		if (buff)
		{
			buff->Update(frame);

			if (buff->IsExpired())
			{
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

void BuffsComponent::RemoveBuff(Buff* buff)
{
	RemoveFromVector(m_Buffs, buff);
	buff->OnRemove();
}

} // namespace raid

