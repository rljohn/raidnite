#include "sandbox/pch.h"
#include "game_instance_widgets.h"

// sandbox
#include "sandbox_imgui.h"
#include "sandbox_game.h"

// engine
#include "engine/game/game.h"
#include "engine/game/game_instance.h"
#include "engine/system/stringutil.h"

namespace raid {
using namespace stringutil;

namespace sandbox {

GameInstanceWidget::GameInstanceWidget()
{
	StringSetEmpty(m_MapName);
}

void GameInstanceWidget::Init()
{
}

void GameInstanceWidget::Draw(GameSandbox* sandbox)
{
	DrawGameStateWidgets(sandbox);
	DrawMapStateWidgets(sandbox);
}

void GameInstanceWidget::Shutdown()
{
}

static const char* LogState(GameState state)
{
	switch (state)
	{
	case GameState::None:
		return "None";
	case GameState::LoadingGame:
		return "Loading";
	case GameState::Active:
		return "Active";
	case GameState::EndGame:
		return "EndGame";
	}

	return "Unknown";
}

void GameInstanceWidget::DrawGameStateWidgets(GameSandbox* sandbox)
{
	GameInstance& instance = sandbox->GetGameInstance();
	ImGui::Text("State: %s", LogState(instance.GetGameState()));
	
	switch (instance.GetGameState())
	{
		case GameState::None:
			DrawGameStateWidgets_None(instance);
			break;
		case GameState::LoadingGame:
			break;
		case GameState::Active:
			break;
		case GameState::EndGame:
			break;
	}
}

void GameInstanceWidget::DrawGameStateWidgets_None(GameInstance& instance)
{
	ImGui::InputText("Map Name", m_MapName, COUNTOF(m_MapName));

	if (ImGui::Button("Begin Game"))
	{

	}
}

void GameInstanceWidget::DrawMapStateWidgets(GameSandbox* sandbox)
{

}

} // namespace sandbox
} // namespace raid