#pragma once

#include "engine/entity/component.h"

#include <list>

namespace raid
{

struct GameEvent;
struct DeathEvent;

class AggroTable : public Component
{
public:

	AggroTable(Entity& parent)
		: Component(parent)
	{
	}

	void AddUnit(Entity* unit);
	void RemoveUnit(Entity* unit);

	void OnGameEvent(const GameEvent& evt) override;

	int Count() const { return (int)m_List.size(); }
	bool Empty() const { return m_List.empty(); }

private:

	void OnUnitDied(const DeathEvent& d);

	std::list<Entity*> m_List;
};

} // namespace raid