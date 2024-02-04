#include "engine/pch.h"
#include "game.h"

#include "engine/entity/entity.h"
#include "engine/game/damage.h"
#include "engine/game/combat.h"

namespace raid
{

IEntityLocator* Game::sm_EntityLocator = nullptr;
IDamageCalculator* Game::sm_DamageCalculator = nullptr;
ICombatSystem* Game::sm_CombatSystem = nullptr;

void Game::SetEntityLocator(IEntityLocator* locator)
{
	sm_EntityLocator = locator;
}

IEntityLocator* Game::GetEntityLocator()
{
	return sm_EntityLocator;
}

void Game::SetDamageCalculator(IDamageCalculator* calculator)
{
	sm_DamageCalculator = calculator;
}

IDamageCalculator* Game::GetDamageCalculator()
{
	return sm_DamageCalculator;
}

void Game::SetCombatSystem(ICombatSystem* system)
{
	sm_CombatSystem = system;
}

ICombatSystem* Game::GetCombatSystem()
{
	return sm_CombatSystem;
}

} // namespace raid