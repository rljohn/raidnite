#pragma once

#include "engine/map/map.h"

#include "widgets.h"

namespace raid {
namespace sandbox {

class MapWidget : public Widget
{
public:

	void Init() override;
	void Draw(GameSandbox* sandbox) override;
	void Shutdown() override;

	const char* GetName() override { return "Map"; }

private:

	void CreateMapWidgets();
	void DrawMapWidgets(GameSandbox* sandbox);
	const char* GetMapIcon(GameSandbox* sandbox, const int x, const int y) const;

	int m_Width = 10;
	int m_Height = 10;

	Map* m_Map = nullptr;
};

} // namespace sandbox
} // namespace raid