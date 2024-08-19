#pragma once

#include "engine/game/game.h"
#include "engine/game/combat_common.h"
#include "engine/game/damage.h"
#include "engine/entity/entity.h"

namespace raid
{

struct GameStartEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::GameStart; }
};

struct GameEndEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::GameEnd; }
};

struct ZoneEnterEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::ZoneEnter; }
};

struct ZoneExitEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::ZoneExit; }
};

struct CombatStartEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::CombatStart; }
};

struct CombatEndEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::CombatEnd; }
};

struct EntityEvent
{
	EntityEvent() = delete;
	EntityEvent(Entity* e)
		: m_Entity(e)
	{
	}

	void SetEntity(Entity* e) { m_Entity = e; }
	Entity* GetEntity() const { return m_Entity; }
	Entity* m_Entity = nullptr;
};

struct UnitSpawnedEvent : public GameEvent, public EntityEvent
{
	virtual GameEventType GetType() const { return GameEventType::UnitSpawned; }

	UnitSpawnedEvent(Entity* e)
		: EntityEvent(e)
	{
	}
};

struct UnitDestroyedEvent : public GameEvent, public EntityEvent
{
	virtual GameEventType GetType() const { return GameEventType::UnitDestroyed; }

	UnitDestroyedEvent(Entity* e)
		: EntityEvent(e)
	{
	}
};

struct UnitOccupancyChangedEvent : public GameEvent, public EntityEvent
{
	virtual GameEventType GetType() const { return GameEventType::UnitOccupancyChanged; }

	UnitOccupancyChangedEvent(Entity* e) : EntityEvent(e) {}
	UnitOccupancyChangedEvent(Entity* e, const Position& position)
		: EntityEvent(e)
		, m_Position(position)
	{
	}

	Position m_Position;
};

struct UnitPositionChangedEvent : public GameEvent, public EntityEvent
{
	virtual GameEventType GetType() const { return GameEventType::UnitPositionChanged; }

	UnitPositionChangedEvent(Entity* e) : EntityEvent(e) {}
	UnitPositionChangedEvent(Entity* e, const Position& previous, const Position& position)
		: EntityEvent(e)
		, m_Previous(previous)
		, m_Position(position)
	{
	}

	Entity* GetEntity() const { return m_Entity; }

	Position m_Previous = InvalidPosition;
	Position m_Position = InvalidPosition;
};

struct DeathEvent : public GameEvent, EntityEvent
{
	virtual GameEventType GetType() const { return GameEventType::Death; }

	DeathEvent(Entity* e)
		: EntityEvent(e)
	{
	}
};

struct DamageEvent : public GameEvent, EntityEvent
{
	GameEventType GetType() const override { return GameEventType::Damage; }

	DamageEvent(Entity* e)
		: EntityEvent(e)
	{
	}

	double BaseDamage = 0;
	double ActualDamage = 0;
	DamageType DamageType = DamageType::Invalid;
};


} // namespace raid