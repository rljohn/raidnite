#pragma once

#include "engine/entity/entity.h"
#include "engine/entity/name.h"
#include "engine/unit/states/state.h"
#include "engine/entity/component.h"
#include "engine/entity/transform.h"
#include "engine/game/damage.h"
#include "engine/unit/ai/ai.h"
#include "engine/unit/aggro.h"
#include "engine/unit/attribute.h"
#include "engine/unit/power.h"
#include "engine/unit/targeting.h"
#include "engine/unit/movement.h"
#include "engine/unit/ai/ai.h"

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

	// Events
	void OnGameEvent(const GameEvent& evt) override;

	// Buff System pass through
	void AddBuff(Buff* buff);
	void CleanseBuff(Buff* buff);
	void RemoveBuff(Buff* buff);

	// Combat Helpers
	bool IsInCombat() const;

	// Easy access to required components
	NameComponent& GetName() { return m_Name; }
	TransformComponent& GetTransform() { return m_Transform; }
	const TransformComponent& GetTransform() const { return m_Transform; }
	StateMachineComponent& GetStateMachine() { return m_StateMachine; }
	AttributesComponent& GetAttributes() { return m_Attributes; }
	
	template<typename T, typename... Args>
	T& AddUnitComponent(Args&&... args)
	{
		static_assert(std::is_base_of<UnitComponent, T>::value,
			"AddComponent type must be derived from UnitComponent");

		T* component = new T(*this, std::forward<Args>(args)...);
		return AddComponent(component);
	}

	// Custom AI
	template<typename T, typename... Args>
	AIComponent* CreateAi(Args&&... args)
	{
		static_assert(std::is_base_of<AIComponent, T>::value, 
			"CreateAi must be derived from AIComponent");

		m_AiController = &AddUnitComponent<T>(std::forward<Args>(args)...);
		return m_AiController;
	}

private:

	///////////////////////
	// Built-In Components
	///////////////////////

	TransformComponent& m_Transform;
	PowerComponent& m_Powers;
	AttributesComponent& m_Attributes;
	BuffsComponent& m_Buffs;
	DamageReceiver& m_DamageReceiver;
	AggroComponent& m_AggroTable;
	NameComponent& m_Name;
	TargetingComponent& m_Targeting;
	StateMachineComponent& m_StateMachine;

	// Dependency: Transform
	MovementComponent& m_Movement;

	///////////////////////
	// Optional Components
	///////////////////////

	// Dependency: Movement
	AIComponent* m_AiController;
};

} // namespace raid