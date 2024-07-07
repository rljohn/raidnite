#pragma once

#include "engine/entity/entity.h"
#include "engine/entity/name.h"
#include "engine/entity/transform.h"
#include "engine/game/damage.h"
#include "engine/unit/aggro.h"
#include "engine/unit/attribute.h"
#include "engine/unit/power.h"
#include "engine/unit/targeting.h"
#include "engine/unit/movement.h"

namespace raid
{

class Tile;

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

	// Movement helpers
	bool OccupyTile(Tile& tile);

	// Combat Helpers
	bool IsInCombat() const;

	// Easy access to required components
	NameComponent& GetName() { return m_Name; }
	TransformComponent& GetTransform() { return m_Transform; }

private:

	PowerComponent& m_Powers;
	AttributesComponent& m_Attributes;
	BuffsComponent& m_Buffs;
	DamageReceiver& m_DamageReceiver;
	AggroTable& m_AggroTable;
	NameComponent& m_Name;
	MovementComponent& m_Movement;
	TransformComponent& m_Transform;
	TargetingComponent& m_Targeting;
};

} // namespace raid