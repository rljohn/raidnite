#pragma once

#include "engine/game/game_instance.h"
#include "widgets.h"

namespace raid {
namespace sandbox {

class GameInstanceWidget : public Widget
{
public:

	GameInstanceWidget();

	void Init() override;
	void Draw(GameSandbox* sandbox) override;
	void Shutdown() override;

	const char* GetName() override { return "Game Instance"; }

private:

	void DrawGameStateWidgets(GameSandbox* sandbox);
	void DrawMapStateWidgets(GameSandbox* sandbox);

	void DrawGameStateWidgets_None(GameInstance& instance);

	char m_MapName[256];
};

} // namespace sandbox
} // namespace raid