#pragma once

#include "engine/entity/component.h"
#include "engine/unit/targeting.h"

#include <list>

namespace raid
{

struct GameEvent;
struct DeathEvent;

enum class AggroBehaviour
{
	Passive,
	Aggro
};

class AggroComponent : public Component
{
public:

	static constexpr const Distance DefaultAggroRange = 5;

	AggroComponent(Entity& parent)
		: Component(parent)
	{
	}

	void AddUnit(Entity* unit);
	void RemoveUnit(Entity* unit);

	void OnGameEvent(const GameEvent& evt) override;

	int Count() const { return (int)m_List.size(); }
	bool Empty() const { return m_List.empty(); }

	Distance GetAggroRange() const { return m_Range; }
	void SetAggroRange(Distance d) { m_Range = d; }

	AggroBehaviour GetBehaviour() const { return m_Behaviour; }
	void SetBehaviour(AggroBehaviour b) { m_Behaviour = b; }

	bool IsHostile() const { return GetBehaviour() == AggroBehaviour::Aggro; }

private:

	void OnUnitDied(const DeathEvent& d);

	std::list<Entity*> m_List;
	Distance m_Range = DefaultAggroRange;
	AggroBehaviour m_Behaviour = AggroBehaviour::Passive;
};

} // namespace raid