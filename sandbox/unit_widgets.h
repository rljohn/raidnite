#pragma once

#include "widgets.h"

#include "engine/types.h"

namespace raid {

class Map;
class StateMachineComponent;
class AttributesComponent;
class AggroComponent;
class TargetingComponent;

namespace sandbox {

class UnitWidget : public Widget
{
public:

	void Init() override;
	void Draw(GameSandbox* sandbox) override;
	void Shutdown() override;

	const char* GetName() override { return "Units"; }

private:

	void DrawSpawnWidgets(GameSandbox* sandbox, Map* map);
	void DrawEntityWidgets(GameSandbox* sandbox, Map* map);
	void DrawEntityState(StateMachineComponent* state);
	void DrawEntityAttributes(AttributesComponent* attrs);
	void DrawAggroTable(AggroComponent* aggro);
	void DrawTargeting(TargetingComponent* targeting);

	std::vector<std::string> m_UnitList;
	int m_UnitListIdx = 0;

	raid::Position m_SpawnPosition;
};

} // namespace sandbox
} // namespace raid