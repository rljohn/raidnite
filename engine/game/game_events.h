#pragma once

#include "engine/game/game.h"
#include "engine/game/combat_common.h"
#include "engine/game/damage.h"
#include "engine/entity/entity.h"

namespace raid
{

// Fwd Declarations
class Tile;

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

struct TilePropertiesChangedEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::TilePropertiesChanged; }
	
	TilePropertiesChangedEvent()
		: m_Position()
		, m_IsValid(false)
		, m_AllowsMovement(false)
		, m_AllowsOccupancy(false)
	{
	}

	TilePropertiesChangedEvent(const Tile* tile);

	TilePropertiesChangedEvent(const Position& pos, bool valid, bool allowsMovement, bool allowsOccupancy)
		: m_Position(pos)
		, m_IsValid(valid)
		, m_AllowsMovement(allowsMovement)
		, m_AllowsOccupancy(allowsOccupancy)
	{
	}

	bool operator==(const TilePropertiesChangedEvent& other) const
	{
		return m_Position == other.m_Position && 
			m_IsValid == other.m_IsValid &&
			m_AllowsMovement == other.m_AllowsMovement &&
			m_AllowsOccupancy == other.m_AllowsOccupancy;
	}

	const Position& GetPosition() const { return m_Position; }
	void SetPosition(const Position& pos) { m_Position = pos; }

	Position m_Position;
	bool m_IsValid;
	bool m_AllowsMovement;
	bool m_AllowsOccupancy;
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

struct EntitySpawnedEvent : public GameEvent, public EntityEvent
{
	virtual GameEventType GetType() const { return GameEventType::EntitySpawned; }

	EntitySpawnedEvent(Entity* e)
		: EntityEvent(e)
	{
	}
};

struct EntityDestroyedEvent : public GameEvent, public EntityEvent
{
	virtual GameEventType GetType() const { return GameEventType::EntityDestroyed; }

	EntityDestroyedEvent(Entity* e)
		: EntityEvent(e)
	{
	}
};

struct EntityOccupancyChangedEvent : public GameEvent, public EntityEvent
{
	virtual GameEventType GetType() const { return GameEventType::EntityOccupancyChanged; }

	EntityOccupancyChangedEvent(Entity* e) : EntityEvent(e) {}
	EntityOccupancyChangedEvent(Entity* e, const Position& position)
		: EntityEvent(e)
		, m_Position(position)
	{
	}

	Position m_Position;
};

struct EntityPositionChangedEvent : public GameEvent, public EntityEvent
{
	virtual GameEventType GetType() const { return GameEventType::EntityPositionChanged; }

	EntityPositionChangedEvent(Entity* e) : EntityEvent(e) {}
	EntityPositionChangedEvent(Entity* e, const Position& previous, const Position& position)
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

	Entity* GetSource() const { return Source; }

	Entity* Source = nullptr;
	double BaseDamage = 0;
	double ActualDamage = 0;
	DamageType DamageType = DamageType::Invalid;
};

struct TargetChangedEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::TargetChanged; }

	TargetChangedEvent(Entity* source, Entity* target)
		: m_Source(source)
		, m_Target(target)
	{
	}

	Entity* m_Source = nullptr;
	Entity* m_Target = nullptr;
};

struct AggroBeginEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::AggroBegin; }

	AggroBeginEvent(Entity* source, Entity* target)
		: m_Source(source)
		, m_Target(target)
	{
	}

	Entity* m_Source = nullptr;
	Entity* m_Target = nullptr;
};

struct AggroEndEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::AggroEnd; }

	AggroEndEvent(Entity* source, Entity* target)
		: m_Source(source)
		, m_Target(target)
	{
	}

	Entity* m_Source = nullptr;
	Entity* m_Target = nullptr;
};

} // namespace raid