#include "engine/pch.h"

#include "ability.h"

#include "engine/engine.h"
#include "engine/game/game.h"

namespace raid
{

//////////////////////////
// Ability
//////////////////////////

bool Ability::IsAvailable() const
{
	return !IsOnCooldown();
}

bool Ability::IsOnCooldown() const
{
	if (m_NextCastFrame == 0)
		return false;

	return GetCurrentGameFrame() < m_NextCastFrame;
}

void Ability::OnCast(Frame frame, double cooldownReduction)
{
	m_LastCastFrame = frame;

	cooldownReduction = std::clamp(cooldownReduction, 0.0, 1.0);

	double cooldown = GetBaseCooldown() * (1.0 - cooldownReduction);

	Duration duration = Time::ToNanoSeconds(cooldown);
	Frame cooldownInFrames = Game::GetEngine()->DurationToFrames(duration);
	m_NextCastFrame = m_LastCastFrame + cooldownInFrames;
}

bool Ability::IsSelfCast() const
{
	return HAS_FLAG(m_Definition.Targeting, AbilityTargetingFlags::Self);
}

bool Ability::IsFriendlyCast() const
{
	return HAS_FLAG(m_Definition.Targeting, AbilityTargetingFlags::Friendly);
}

bool Ability::IsGroundCast() const
{
	return HAS_FLAG(m_Definition.Targeting, AbilityTargetingFlags::Ground);
}

bool Ability::IsEnemyCast() const
{
	return HAS_FLAG(m_Definition.Targeting, AbilityTargetingFlags::EnemyTarget);
}

//////////////////////////
// Ability
//////////////////////////

void AbilityComponent::AddAbility(const AbilityDefinition& def)
{
	mainAssert(GetAbility(def.Id) == nullptr);
	m_Abilities.push_back(new Ability(def));
}

Ability* AbilityComponent::GetAbility(AbilityId ability)
{
	for (Ability* a : m_Abilities)
	{
		if (a->GetId() == ability)
		{
			return a;
		}
	}

	return nullptr;
}

void AbilityComponent::SetCurrentAbility(Ability* ability)
{
	m_CurrentAbility = ability;
}

} // namespace raid
