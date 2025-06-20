#include "engine/pch.h"
#include "combat.h"
#include "engine/types.h"
#include "engine/unit/unit.h"
#include "engine/unit/unit_utils.h"
#include "engine/system/check.h"

namespace raid
{

void CombatSystem::BuildDamageEvent(const DamageParams& e)
{
	stry
	{
		// Locator is required to find the source/target units.
		IEntityManager* locator = Game::GetEntityManager();
		scheckall(locator);

		// Comgat resolver is required to calculate damage
		IDamageCalculator* calculator = Game::GetDamageCalculator();
		scheckall(calculator);

		// Target is required to apply damage
		Entity* target = locator->FindEntity(e.Target);
		scheckall(target);

		// If the source entity is still alive, retrieve it.
		// Fallback to the provided value if unavailable.
		double sourceLevel = e.SourceLevel;
		Entity* source = locator->FindEntity(e.Source);
		if (source)
		{
			double level;
			if (UnitStatics::GetUnitLevel(*source, level))
			{
				sourceLevel = level;
			}
		}

		double resistPercent = 0;
		switch (e.Type)
		{
			case DamageType::Physical:
			{
				double armor;
				if (UnitStatics::GetAttribute<AttributeType::Armor>(*target, armor))
				{
					resistPercent = calculator->CalculateArmorResistPercent(sourceLevel, armor);
				}
			}
			break;

			case DamageType::Fire:
			{
				double armor;
				if (UnitStatics::GetAttribute<AttributeType::FireResist>(*target, armor))
				{
					resistPercent = calculator->CalculateResistPercent(sourceLevel, armor);
				}
			}
			break;

			case DamageType::Frost:
			{
				double armor;
				if (UnitStatics::GetAttribute<AttributeType::FrostResist>(*target, armor))
				{
					resistPercent = calculator->CalculateResistPercent(sourceLevel, armor);
				}
			}
			break;

			case DamageType::Earth:
			{
				double armor;
				if (UnitStatics::GetAttribute<AttributeType::EarthResist>(*target, armor))
				{
					resistPercent = calculator->CalculateResistPercent(sourceLevel, armor);
				}
			}
			break;
		}

		double baseDamage = e.Value;
		double actualDamage = (1.0f - resistPercent) * baseDamage;

		DamageEvent evt(target);
		evt.Source = source;
		evt.BaseDamage = baseDamage;
		evt.ActualDamage = actualDamage;

		if (DamageReceiver* receiver = target->GetComponent<DamageReceiver>())
		{
			receiver->ApplyDamage(evt);
		}

		Game::DispatchGameEvent(evt);
	}
	scatchall
	{

	}	
}

void CombatSystem::KillEntity(Entity* e)
{
	if (IEntityManager* mgr = Game::GetEntityManager()) 
	{
		DeathEvent evt(e);
		mgr->OnGameEvent(evt);
	}
}

CombatSystemRAII::CombatSystemRAII()
{
	Game::SetCombatSystem(&Instance);
}

CombatSystemRAII::~CombatSystemRAII()
{
	Game::SetCombatSystem(nullptr);
}

} // namespace raid