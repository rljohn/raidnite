#pragma once

#include "engine/game/game_instance.h"
#include "widgets.h"

namespace raid {
namespace sandbox {

class LoadDlgt : public raid::ILoadDelegate
{
	bool LoadGameData(const LoadContext& ctx)
	{
		// no-op currently in sandbox
		return true;
	}
};

class GameInstanceWidget : public Widget
{
public:

	GameInstanceWidget();

	void Init() override;
	void Draw(GameSandbox* sandbox) override;
	void Shutdown() override;

	const char* GetName() override { return "GameInstance"; }

private:

	void DrawGameStateWidgets(GameSandbox* sandbox);

	void DrawGameStateWidgets_None(GameInstance& instance);
	void DrawGameStateWidgets_Loading(GameInstance& instance);
	void DrawGameStateWidgets_Active(GameInstance& instance);
	void DrawGameStateWidgets_End(GameInstance& instance);

	char m_MapName[256];
	std::shared_ptr<LoadDlgt> m_LoadDlgt;
};

} // namespace sandbox
} // namespace raid