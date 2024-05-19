#include "sandbox/pch.h"
#include "game_instance_widgets.h"

// sandbox
#include "sandbox_imgui.h"
#include "sandbox_game.h"

// engine
#include "engine/game/game.h"
#include "engine/game/game_instance.h"
#include "engine/game/loading.h"
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
	SetEnabled(true);
	m_LoadDlgt = std::make_shared<LoadDlgt>();
}

void GameInstanceWidget::Draw(GameSandbox* sandbox)
{
	DrawGameStateWidgets(sandbox);
}

void GameInstanceWidget::Shutdown()
{
	m_LoadDlgt = nullptr;
}

static const char* LogState(GameState state)
{
	switch (state)
	{
	case GameState::None:
		return "None";
		case GameState::Loading:
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
		case GameState::Loading:
			DrawGameStateWidgets_Loading(instance);
			break;
		case GameState::Active:
			DrawGameStateWidgets_Active(instance);
			break;
		case GameState::EndGame:
			DrawGameStateWidgets_End(instance);
			break;
	}
}

void GameInstanceWidget::DrawGameStateWidgets_None(GameInstance& instance)
{
	ImGui::InputText("Map Name", m_MapName, COUNTOF(m_MapName));

	if (ImGui::Button("Begin Game"))
	{
		instance.Init(std::static_pointer_cast<raid::ILoadDelegate>(m_LoadDlgt));

		raid::LoadContext ctx;
		ctx.MapName = m_MapName;
		ctx.LevelScript = "";
		instance.BeginLoadGame(ctx);
	}
}

void GameInstanceWidget::DrawGameStateWidgets_Loading(GameInstance& instance)
{
	if (!m_LoadDlgt->IsMapLoaded())
	{
		if (ImGui::Button("OnMapLoaded"))
		{
			m_LoadDlgt->OnMapLoaded();
		}
	}
	else if (!m_LoadDlgt->IsGameLoaded())
	{
		if (ImGui::Button("OnGameLoaded"))
		{
			m_LoadDlgt->OnGameLoaded();
		}
	}
}

void GameInstanceWidget::DrawGameStateWidgets_Active(GameInstance& instance)
{
	if (ImGui::Button("End Game"))
	{
		raid::LoadContext ctx;
		instance.BeginEndGame(ctx);
	}
}

void GameInstanceWidget::DrawGameStateWidgets_End(GameInstance& instance)
{
	if (ImGui::Button("Reset"))
	{
		instance.Reset();
	}
}


} // namespace sandbox
} // namespace raid